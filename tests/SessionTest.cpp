/**
 * @file SessionTest.cpp
 * @brief Tests that every TUI action does what the screen says it does.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include <cstdio>

#include "Scenarios.h"
#include "Session.h"
#include "State.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"

namespace
{
    /**
     * @brief Whether a case has a piece of added work, and who has it.
     * @param session The session.
     * @param c The case.
     * @param name The work's name.
     * @return The stage it is in, or empty if it is not there.
     */
    std::string stageOf(Session &session, WorkflowInstance *c, const std::string &name)
    {
        for (const StageView &stage : session.stages(c))
        {
            for (const WorkView &work : stage.work)
            {
                if (work.name == name)
                {
                    return stage.name;
                }
            }
        }
        return "";
    }
}

// ----- setup -----

TEST(SessionTest, BuildsTheStaffAndDefinitionFromTheSetup)
{
    Session session(Scenarios::hospital());

    EXPECT_EQ(session.roster().size(), 5u);
    EXPECT_EQ(session.definition()->getName(), "Patient admission");
    EXPECT_EQ(session.definition()->getStages()->size(), 5u);
    EXPECT_EQ(session.definition()->getRoutingStrategyType(), "EventBased");
}

TEST(SessionTest, ChainFollowsTheSetupOrder)
{
    Session session(Scenarios::hospital());
    std::vector<ChainStep> chain = session.chain();

    ASSERT_EQ(chain.size(), 4u);
    EXPECT_EQ(chain.front().name, "Ann Botha");
    EXPECT_EQ(chain.back().name, "Dr Mark Steyn");
}

TEST(SessionTest, StaffTreeIsDrawnByTheEngine)
{
    Session session(Scenarios::shield());
    std::vector<std::string> tree = session.staffTree();

    ASSERT_FALSE(tree.empty());
    EXPECT_EQ(tree.front(), "Team: Field Agents (Rank: 1)");
}

// ----- cases -----

TEST(SessionTest, NewCaseIsActiveWithTheStartingValues)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getState()->getName(), "Active");
    EXPECT_EQ(c->getData("triage score"), 3);
    EXPECT_EQ(session.statePath(c), (std::vector<std::string>{"Pending", "Active"}));
}

TEST(SessionTest, ApproveCompletesAnActiveCase)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    ActionResult result = session.approve(c);
    EXPECT_TRUE(result.changed);
    EXPECT_EQ(c->getState()->getName(), "Completed");
    EXPECT_EQ(session.undoDepth(c), 1u);
}

TEST(SessionTest, RefusedActionChangesNothingAndCannotBeUndone)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    session.approve(c);

    ActionResult again = session.approve(c);
    EXPECT_FALSE(again.changed);
    EXPECT_EQ(c->getState()->getName(), "Completed");
    EXPECT_EQ(session.undoDepth(c), 1u);
    EXPECT_TRUE(session.timeline(c).back().refused);
}

TEST(SessionTest, RejectThenRetryGoesBackAndForth)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    session.reject(c);
    EXPECT_EQ(c->getState()->getName(), "Pending");
    session.retry(c);
    EXPECT_EQ(c->getState()->getName(), "Active");
}

TEST(SessionTest, UndoRestoresEachEarlierStateInTurn)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    session.reject(c);  // Active -> Pending
    session.approve(c); // Pending -> Active
    session.approve(c); // Active -> Completed

    session.undo(c);
    EXPECT_EQ(c->getState()->getName(), "Active");
    session.undo(c);
    EXPECT_EQ(c->getState()->getName(), "Pending");
    session.undo(c);
    EXPECT_EQ(c->getState()->getName(), "Active");
    EXPECT_FALSE(session.undo(c).changed);
}

TEST(SessionTest, CancelCanBeUndone)
{
    Session session(Scenarios::insurance());
    WorkflowInstance *c = session.openCase();

    session.cancel(c);
    EXPECT_EQ(c->getState()->getName(), "Cancelled");
    session.undo(c);
    EXPECT_EQ(c->getState()->getName(), "Active");
}

TEST(SessionTest, ExecuteRunsThroughTheFacade)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    EXPECT_TRUE(session.execute(c).changed);
    EXPECT_EQ(c->getState()->getName(), "Completed");
    EXPECT_FALSE(session.execute(c).changed);
}

TEST(SessionTest, StatePathFollowsTheEventLog)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    session.reject(c);
    session.approve(c);
    session.undo(c);

    EXPECT_EQ(session.statePath(c), (std::vector<std::string>{"Pending", "Active", "Pending", "Active", "Pending"}));
}

TEST(SessionTest, CasesDoNotAffectEachOther)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *first = session.openCase();
    WorkflowInstance *second = session.openCase();

    session.cancel(first);
    session.setData(first, "triage score", 9);
    session.addWork(first, {"Check vitals", "Nurse", "Low"}, "RoleBased", "EventBased");

    EXPECT_EQ(second->getState()->getName(), "Active");
    EXPECT_EQ(second->getData("triage score"), 3);
    EXPECT_EQ(stageOf(session, second, "Check vitals"), "");
    EXPECT_EQ(session.undoDepth(second), 0u);
}

// ----- work -----

TEST(SessionTest, AddedWorkGoesToTheRightPersonAndStage)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    SubmitResult result = session.addWork(c, {"Check vitals", "Nurse", "Low"}, "RoleBased", "EventBased");
    ASSERT_TRUE(result.accepted);
    EXPECT_EQ(result.assigneeTitle, "Nurse");
    EXPECT_EQ(result.stage, "Assessment"); // triage 3 is below 4
    EXPECT_EQ(result.notificationsSent, 2);
    EXPECT_EQ(stageOf(session, c, "Check vitals"), "Assessment");
}

TEST(SessionTest, ChangingTheCaseValueChangesWhereWorkGoes)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    session.setData(c, "triage score", 9);

    SubmitResult result = session.addWork(c, {"Check vitals", "Nurse", "Low"}, "RoleBased", "EventBased");
    EXPECT_EQ(result.stage, "Emergency Care");
}

TEST(SessionTest, ApprovalChainEscalatesAboveTheLimit)
{
    Session session(Scenarios::insurance());
    WorkflowInstance *c = session.openCase();

    EXPECT_EQ(session.addWork(c, {"Small claim", "Assessor", "Low"}, "RoleBased", "ApprovalChain").stage, "Assessment");
    session.setData(c, "claim amount", 80000);
    EXPECT_EQ(session.addWork(c, {"Big claim", "Assessor", "Low"}, "RoleBased", "ApprovalChain").stage, "Senior Review");
}

TEST(SessionTest, SeverityRoutingSendsCriticalWorkToTheEmergencyStage)
{
    Session session(Scenarios::shield());
    WorkflowInstance *c = session.openCase();

    EXPECT_EQ(session.addWork(c, {"Scan", "Agent", "Low"}, "RoleBased", "SeverityBased").stage, "Deploy Team");
    EXPECT_EQ(session.addWork(c, {"Tesseract", "Agent", "Critical"}, "RoleBased", "SeverityBased").stage,
              "Assemble Avengers");
}

TEST(SessionTest, WorkNobodyFitsIsRefusedAndNotAdded)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    SubmitResult result = session.addWork(c, {"Fly the plane", "Pilot", "Low"}, "RoleBased", "EventBased");
    EXPECT_FALSE(result.accepted);
    EXPECT_EQ(result.notificationsSent, 1);
    EXPECT_EQ(stageOf(session, c, "Fly the plane"), "");
}

TEST(SessionTest, WorkloadAssignmentSpreadsTheWork)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    std::string first = session.addWork(c, {"One", "", "Low"}, "WorkloadBased", "EventBased").assignee;
    std::string second = session.addWork(c, {"Two", "", "Low"}, "WorkloadBased", "EventBased").assignee;
    EXPECT_NE(first, second);
}

TEST(SessionTest, RuleAssignmentUsesTheRules)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    SubmitResult result = session.addWork(c, {"Order tests", "Doctor", "Low"}, "RuleBased", "EventBased");
    EXPECT_EQ(result.assigneeTitle, "Doctor");
}

TEST(SessionTest, NewPersonCanBeGivenWork)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();

    EXPECT_TRUE(session.addPerson({"Lindiwe Zulu", "Pharmacist", 2, "Ward"}).changed);
    SubmitResult result = session.addWork(c, {"Dispense", "Pharmacist", "Low"}, "RoleBased", "EventBased");
    EXPECT_EQ(result.assignee, "Lindiwe Zulu");
}

TEST(SessionTest, DuplicateOrTeamlessPersonIsRefused)
{
    Session session(Scenarios::hospital());

    EXPECT_FALSE(session.addPerson({"Ann Botha", "Clerk", 1, ""}).changed);
    EXPECT_FALSE(session.addPerson({"New", "Clerk", 1, "No Such Team"}).changed);
}

// ----- escalation -----

TEST(SessionTest, EscalationStopsAtTheFirstPersonWithTheRank)
{
    Session session(Scenarios::hospital());
    EscalationResult result = session.escalate(nullptr, 3);

    EXPECT_TRUE(result.handled);
    ASSERT_EQ(result.steps.size(), 3u);
    EXPECT_FALSE(result.steps[0].canHandle);
    EXPECT_EQ(result.steps.back().name, "Dr Priya Naidoo");
    EXPECT_TRUE(result.steps.back().canHandle);
}

TEST(SessionTest, EscalationAboveEveryoneIsNotHandled)
{
    Session session(Scenarios::hospital());
    EscalationResult result = session.escalate(nullptr, 9);

    EXPECT_FALSE(result.handled);
    EXPECT_EQ(result.steps.size(), 4u);
}

TEST(SessionTest, EscalatingWorkStartsWithItsPerson)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    session.addWork(c, {"Check vitals", "Nurse", "Low"}, "RoleBased", "EventBased");

    EscalationResult result = session.escalateWork(c, "Check vitals", 4);
    EXPECT_TRUE(result.handled);
    EXPECT_EQ(result.steps.front().title, "Nurse");
    EXPECT_EQ(result.steps.back().name, "Dr Mark Steyn");
}

// ----- reports and output -----

TEST(SessionTest, ReportsDescribeTheCase)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    session.addWork(c, {"Check vitals", "Nurse", "Low"}, "RoleBased", "EventBased");

    std::string progress = session.progressReport(c);
    EXPECT_NE(progress.find("Stage: Admission"), std::string::npos);
    EXPECT_NE(progress.find("Check vitals"), std::string::npos);
    EXPECT_NE(session.auditTrail(c).find("STATE_CHANGE"), std::string::npos);
}

TEST(SessionTest, HistoryIsSavedToCsv)
{
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    std::string path = "session_test_history.csv";

    EXPECT_TRUE(session.saveHistory(c, path).changed);
    std::remove(path.c_str());
}

TEST(SessionTest, EngineOutputIsKeptNotPrinted)
{
    testing::internal::CaptureStdout();
    Session session(Scenarios::hospital());
    WorkflowInstance *c = session.openCase();
    session.approve(c);
    std::string leaked = testing::internal::GetCapturedStdout();

    EXPECT_EQ(leaked, "");
    bool sawState = false;
    for (const std::string &line : session.engineLog())
    {
        sawState = sawState || line.find("[State]") != std::string::npos;
    }
    EXPECT_TRUE(sawState);
}

TEST(SessionTest, EveryReadyMadeScenarioRunsEndToEnd)
{
    for (const ScenarioSetup &setup : Scenarios::all())
    {
        Session session(setup);
        WorkflowInstance *c = session.openCase();
        for (const WorkSpec &work : setup.sampleWork)
        {
            EXPECT_TRUE(session.addWork(c, work, setup.assignment, setup.routing.kind).accepted)
                << setup.name << ": " << work.name;
        }
        EXPECT_TRUE(session.approve(c).changed) << setup.name;
    }
}
