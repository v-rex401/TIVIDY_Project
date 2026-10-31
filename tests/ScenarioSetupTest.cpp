/**
 * @file ScenarioSetupTest.cpp
 * @brief Tests that ScenarioSetup catches setups that cannot work, and that the ready-made ones can.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include "ScenarioSetup.h"
#include "Scenarios.h"

namespace
{
    /**
     * @brief The smallest setup that works: one person, one stage, severity routing.
     * @return The setup.
     */
    ScenarioSetup smallest()
    {
        ScenarioSetup s;
        s.workflowName = "Tiny";
        s.people = {{"Jane", "Nurse", 2, ""}};
        s.stages = {{"Ward", {}}};
        s.routing.kind = "SeverityBased";
        s.routing.normalStage = "Ward";
        s.routing.emergencyStage = "Ward";
        return s;
    }

    /**
     * @brief Whether any problem mentions a piece of text.
     * @param problems The problems found.
     * @param text The text to look for.
     * @return True if one of them contains it.
     */
    bool mentions(const std::vector<std::string> &problems, const std::string &text)
    {
        for (const std::string &p : problems)
        {
            if (p.find(text) != std::string::npos)
            {
                return true;
            }
        }
        return false;
    }
}

TEST(ScenarioSetupTest, SmallestSetupHasNoProblems)
{
    EXPECT_TRUE(smallest().problems().empty());
}

TEST(ScenarioSetupTest, EmptySetupAsksForNameStagesAndPeople)
{
    ScenarioSetup empty;
    std::vector<std::string> problems = empty.problems();
    EXPECT_TRUE(mentions(problems, "name"));
    EXPECT_TRUE(mentions(problems, "stage"));
    EXPECT_TRUE(mentions(problems, "person"));
}

TEST(ScenarioSetupTest, PersonInAMissingTeamIsAProblem)
{
    ScenarioSetup s = smallest();
    s.people[0].team = "Ghost Ward";
    EXPECT_TRUE(mentions(s.problems(), "Ghost Ward"));
}

TEST(ScenarioSetupTest, ChainMemberWhoIsNotSetUpIsAProblem)
{
    ScenarioSetup s = smallest();
    s.chain = {"Jane", "Nobody"};
    EXPECT_TRUE(mentions(s.problems(), "Nobody"));
}

TEST(ScenarioSetupTest, RoutingToAMissingStageIsAProblem)
{
    ScenarioSetup s = smallest();
    s.routing.emergencyStage = "ICU";
    EXPECT_TRUE(mentions(s.problems(), "ICU"));
}

TEST(ScenarioSetupTest, EventRoutingWithoutThresholdsIsAProblem)
{
    ScenarioSetup s = smallest();
    s.routing.kind = "EventBased";
    s.routing.metricKey = "score";
    EXPECT_TRUE(mentions(s.problems(), "threshold"));
}

TEST(ScenarioSetupTest, DuplicateNamesAreAProblem)
{
    ScenarioSetup s = smallest();
    s.people.push_back({"Jane", "Doctor", 3, ""});
    EXPECT_TRUE(mentions(s.problems(), "Jane"));
}

TEST(ScenarioSetupTest, EveryReadyMadeScenarioIsUsable)
{
    for (const ScenarioSetup &s : Scenarios::all())
    {
        EXPECT_TRUE(s.problems().empty()) << s.name << ": " << (s.problems().empty() ? "" : s.problems()[0]);
        EXPECT_FALSE(s.sampleWork.empty()) << s.name;
    }
}
