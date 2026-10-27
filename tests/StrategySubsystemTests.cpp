#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "ApprovalChainRouting.h"
#include "EventBasedRouting.h"
#include "RoleBasedAssignment.h"
#include "RuleBasedAssignment.h"
#include "SeverityBasedRouting.h"
#include "Threshold.h"
#include "WorkloadBasedAssignment.h"
#include "WorkflowManager.h"
#include "WorkflowDefinition.h"
#include "Cancelled.h"

class TestStaff : public Staff
{
private:
    std::string name, title;
    int rank;

public:
    TestStaff(const std::string &name, const std::string &title, int rank = 1) : name(name), title(title), rank(rank) {}
    std::string getName() override { return name; }
    std::string getTitle() override { return title; }
    int getRank() override { return rank; }
    bool handleRequest(int /*rank*/) override { return true; }
};

class StrategyTestWorkItem : public WorkItem
{
private:
    std::string name, title, severity, required;
    Staff *responsible;

public:
    StrategyTestWorkItem(const std::string &name = "item", const std::string &title = "task", const std::string &severity = "Normal", const std::string &required = "Doctor")
        : name(name), title(title), severity(severity), required(required), responsible(nullptr) {}
    std::string getName() override { return name; }
    std::string getTitle() override { return title; }
    std::string getSeverity() override { return severity; }
    std::string getRequiredPosition() override { return required; }
    void setRequiredPosition(const std::string &p) override { required = p; }
    void printHierarchy(std::string) override {}
    bool handleRequest(int /*rank*/) override { return true; }
    WorkItem *clone() override { return new StrategyTestWorkItem(*this); }
    Staff *getResponsible() override { return responsible; }
    void setResponsible(Staff *s) override { responsible = s; }
    void setSeverity(const std::string &s) { severity = s; }
};

class RecordingAssignment : public AssignmentStrategy
{
private:
    Staff *result;
    int *destroyed;

public:
    explicit RecordingAssignment(Staff *result, int *destroyed = nullptr) : calls(0), lastRunningCount(0), result(result), destroyed(destroyed) {}
    ~RecordingAssignment() override
    {
        if (destroyed)
            ++*destroyed;
    }
    Staff *selectAssignmentItem(WorkItem *, WorkflowInstance *, const std::vector<WorkflowInstance *> &running) override
    {
        ++calls;
        lastRunningCount = running.size();
        return result;
    }
    int calls;
    std::size_t lastRunningCount;
};

class RecordingRouting : public RoutingStrategy
{
private:
    Stage *result;
    int *destroyed;

public:
    explicit RecordingRouting(Stage *result, int *destroyed = nullptr) : calls(0), result(result), destroyed(destroyed) {}
    ~RecordingRouting() override
    {
        if (destroyed)
            ++*destroyed;
    }
    Stage *selectItemState(WorkItem *, WorkflowInstance *) override
    {
        ++calls;
        return result;
    }
    int calls;
};

TEST(RoleBasedAssignmentTests, SelectsFirstMatchingRequiredPositionAndSkipsNulls)
{
    TestStaff doctor("D1", "Doctor"), nurse("N1", "Nurse"), doctor2("D2", "Doctor");
    StrategyTestWorkItem item("x", "unrelated title", "Normal", "Doctor");
    RoleBasedAssignment strategy({nullptr, &nurse, &doctor, &doctor2});
    EXPECT_EQ(&doctor, strategy.selectAssignmentItem(&item, nullptr, {}));
}
TEST(RoleBasedAssignmentTests, NullItemAndNoMatchingRoleReturnNull)
{
    TestStaff nurse("N1", "Nurse");
    StrategyTestWorkItem item("x", "x", "Normal", "Doctor");
    RoleBasedAssignment strategy({&nurse});
    EXPECT_EQ(nullptr, strategy.selectAssignmentItem(nullptr, nullptr, {}));
    EXPECT_EQ(nullptr, strategy.selectAssignmentItem(&item, nullptr, {}));
}

TEST(RuleBasedAssignmentTests, EvaluatesRulesInOrderAndSelectsMatchingRosterTitle)
{
    TestStaff doctor("D", "Doctor"), nurse("N", "Nurse");
    StrategyTestWorkItem item("urgent", "review", "High", "Doctor");
    AssignmentRule wrong{"name", "other", "Nurse"};
    AssignmentRule right{"requiredPosition", "Doctor", "Doctor"};
    RuleBasedAssignment strategy({wrong, right}, {nullptr, &nurse, &doctor});
    EXPECT_EQ(&doctor, strategy.selectAssignmentItem(&item, nullptr, {}));
}
TEST(RuleBasedAssignmentTests, UnmatchedRuleNullItemAndMissingTargetReturnNull)
{
    TestStaff doctor("D", "Doctor");
    StrategyTestWorkItem item("urgent", "review", "High", "Doctor");
    RuleBasedAssignment mismatch({AssignmentRule{"name", "nope", "Doctor"}}, {&doctor});
    RuleBasedAssignment missingTarget({AssignmentRule{"name", "urgent", "Nurse"}}, {&doctor});
    EXPECT_EQ(nullptr, mismatch.selectAssignmentItem(&item, nullptr, {}));
    EXPECT_EQ(nullptr, missingTarget.selectAssignmentItem(&item, nullptr, {}));
    EXPECT_EQ(nullptr, mismatch.selectAssignmentItem(nullptr, nullptr, {}));
}

TEST(WorkloadBasedAssignmentTests, ChoosesLeastLoadedStaffAcrossRunningInstances)
{
    TestStaff a("A", "Doctor"), b("B", "Doctor");
    StrategyTestWorkItem assignedA1, assignedA2, candidate;
    assignedA1.setResponsible(&a);
    assignedA2.setResponsible(&a);
    WorkflowInstance one, two, context;
    one.addWorkItem(&assignedA1);
    two.addWorkItem(&assignedA2);
    WorkloadBasedAssignment strategy({&a, &b});
    EXPECT_EQ(&b, strategy.selectAssignmentItem(&candidate, &context, {&one, &two}));
}
TEST(WorkloadBasedAssignmentTests, TiesChooseFirstRosterMemberAndNullInputsReturnNull)
{
    TestStaff a("A", "Doctor"), b("B", "Doctor");
    StrategyTestWorkItem candidate;
    WorkflowInstance context;
    WorkloadBasedAssignment strategy({nullptr, &a, &b});
    EXPECT_EQ(&a, strategy.selectAssignmentItem(&candidate, &context, {}));
    EXPECT_EQ(nullptr, strategy.selectAssignmentItem(nullptr, &context, {}));
    EXPECT_EQ(nullptr, strategy.selectAssignmentItem(&candidate, nullptr, {}));
    WorkloadBasedAssignment empty({});
    EXPECT_EQ(nullptr, empty.selectAssignmentItem(&candidate, &context, {}));
    WorkflowInstance running;
    WorkloadBasedAssignment withNullInstance({&a});
    EXPECT_EQ(&a, withNullInstance.selectAssignmentItem(&candidate, &context, {nullptr, &running}));
}

TEST(SeverityBasedRoutingTests, RoutesCriticalAndHighToEmergencyOtherwiseNormal)
{
    Stage normal, emergency;
    WorkflowInstance context;
    SeverityBasedRouting strategy(&normal, &emergency);
    StrategyTestWorkItem item;
    EXPECT_EQ(&normal, strategy.selectItemState(&item, &context));
    item.setSeverity("High");
    EXPECT_EQ(&emergency, strategy.selectItemState(&item, &context));
    item.setSeverity("Critical");
    EXPECT_EQ(&emergency, strategy.selectItemState(&item, &context));
    item.setSeverity("Unknown");
    EXPECT_EQ(&normal, strategy.selectItemState(&item, &context));
    EXPECT_EQ(nullptr, strategy.selectItemState(nullptr, &context));
    EXPECT_EQ(nullptr, strategy.selectItemState(&item, nullptr));
}
TEST(ApprovalChainRoutingTests, ThresholdIsStrictAndUsesContextTransactionValue)
{
    Stage standard, escalation;
    WorkflowInstance context;
    StrategyTestWorkItem item;
    ApprovalChainRouting strategy("transactionValue", 100.0, &standard, &escalation);
    context.setData("transactionValue", 99.99);
    EXPECT_EQ(&standard, strategy.selectItemState(&item, &context));
    context.setData("transactionValue", 100.0);
    EXPECT_EQ(&standard, strategy.selectItemState(&item, &context));
    context.setData("transactionValue", 100.01);
    EXPECT_EQ(&escalation, strategy.selectItemState(&item, &context));
    EXPECT_EQ(nullptr, strategy.selectItemState(nullptr, &context));
    EXPECT_EQ(nullptr, strategy.selectItemState(&item, nullptr));
    SeverityBasedRouting noEmergency(nullptr, nullptr);
    item.setSeverity("Critical");
    EXPECT_EQ(nullptr, noEmergency.selectItemState(&item, &context));
}
TEST(ApprovalChainRoutingTests, NullDestinationStagesAreReturnedAsConfigured)
{
    WorkflowInstance context;
    StrategyTestWorkItem item;
    ApprovalChainRouting standardMissing("transactionValue", 100.0, nullptr, nullptr);
    EXPECT_EQ(nullptr, standardMissing.selectItemState(&item, &context));
    context.setData("transactionValue", 101.0);
    EXPECT_EQ(nullptr, standardMissing.selectItemState(&item, &context));
}
TEST(EventBasedRoutingTests, FirstTriggeredThresholdWinsInConfiguredOrder)
{
    Stage first, second, third;
    WorkflowInstance context;
    StrategyTestWorkItem item;
    EventBasedRouting strategy("eventMetric", {Threshold(50.0, &first), Threshold(80.0, &second), Threshold(20.0, &third)});

    context.setData("eventMetric", 90.0);
    EXPECT_EQ(&first, strategy.selectItemState(&item, &context));

    // Checked while a threshold is triggered, so a null argument can only be
    // rejected deliberately rather than because nothing matched.
    EXPECT_EQ(nullptr, strategy.selectItemState(nullptr, &context));
    EXPECT_EQ(nullptr, strategy.selectItemState(&item, nullptr));

    context.setData("eventMetric", 40.0);
    EXPECT_EQ(&third, strategy.selectItemState(&item, &context));

    context.setData("eventMetric", 10.0);
    EXPECT_EQ(nullptr, strategy.selectItemState(&item, &context));

    EventBasedRouting empty("eventMetric", {});
    EXPECT_EQ(nullptr, empty.selectItemState(&item, &context));
}

TEST(WorkflowManagerTests, MissingStrategiesAndNullInputsReturnNull)
{
    WorkflowManager manager;
    WorkflowInstance context;
    StrategyTestWorkItem item;
    EXPECT_EQ(nullptr, manager.assignWorkItem(&item, &context));
    EXPECT_EQ(nullptr, manager.routeWorkItem(&item, &context));
    EXPECT_EQ(nullptr, manager.assignWorkItem(nullptr, &context));
    EXPECT_EQ(nullptr, manager.routeWorkItem(&item, nullptr));
}
TEST(WorkflowManagerTests, DelegatesAssignmentAndRoutingAndOwnsReplacedStrategies)
{
    TestStaff staff("A", "Doctor");
    Stage destination;
    WorkflowManager manager;
    int assignmentDestroyed = 0, routingDestroyed = 0;
    manager.setAssignmentStrategy(new RecordingAssignment(&staff, &assignmentDestroyed));
    manager.setAssignmentStrategy(new RecordingAssignment(&staff, &assignmentDestroyed));
    EXPECT_EQ(1, assignmentDestroyed);
    manager.setRoutingStrategy(new RecordingRouting(&destination, &routingDestroyed));
    manager.setRoutingStrategy(new RecordingRouting(&destination, &routingDestroyed));
    EXPECT_EQ(1, routingDestroyed);
    WorkflowInstance context;
    StrategyTestWorkItem item;
    EXPECT_EQ(&staff, manager.assignWorkItem(&item, &context));
    EXPECT_EQ(&destination, manager.routeWorkItem(&item, &context));
    manager.setAssignmentStrategy(nullptr);
    manager.setRoutingStrategy(nullptr);
    EXPECT_EQ(2, assignmentDestroyed);
    EXPECT_EQ(2, routingDestroyed);
}
TEST(WorkflowManagerTests, CreateInstanceSetsIdentifierAndRejectsNullDefinition)
{
    WorkflowManager manager;
    WorkflowDefinition definition;
    EXPECT_EQ(nullptr, manager.createInstance(nullptr, "bad"));
    WorkflowInstance *instance = manager.createInstance(&definition, "wf-123");
    ASSERT_NE(nullptr, instance);
    EXPECT_EQ("wf-123", instance->getId());
    delete instance; // caller owns the returned instance
}
TEST(WorkflowManagerTests, StartIsIdempotentAndExecuteAssignsRoutesAndRecordsItem)
{
    TestStaff staff("A", "Doctor");
    Stage destination;
    WorkflowManager manager;
    RecordingAssignment *assignment = new RecordingAssignment(&staff);
    RecordingRouting *routing = new RecordingRouting(&destination);
    manager.setAssignmentStrategy(assignment);
    manager.setRoutingStrategy(routing);
    WorkflowInstance context;
    // Routing hands the item to a Stage, which owns its members and destroys
    // them, so it must be heap-allocated.
    StrategyTestWorkItem *item = new StrategyTestWorkItem();
    manager.startWorkflow(nullptr);
    manager.startWorkflow(&context);
    manager.startWorkflow(&context);
    manager.executeWorkflow(&context, item);
    EXPECT_EQ(&staff, item->getResponsible());
    ASSERT_EQ(1u, context.getWorkItems().size());
    EXPECT_EQ(item, context.getWorkItems().front());
    ASSERT_EQ(1u, destination.getMembers().size());
    EXPECT_EQ(item, destination.getMembers().front());
    EXPECT_EQ(1, assignment->calls);
    EXPECT_EQ(1, routing->calls);
    EXPECT_EQ(1u, assignment->lastRunningCount);
    manager.executeWorkflow(nullptr, item);
    manager.executeWorkflow(&context, nullptr);
    EXPECT_EQ(1, assignment->calls);
}
TEST(WorkflowManagerTests, FailedAssignmentOrRouteDoesNotRecordItem)
{
    Stage destination;
    WorkflowManager manager;
    manager.setAssignmentStrategy(new RecordingAssignment(nullptr));
    manager.setRoutingStrategy(new RecordingRouting(&destination));
    WorkflowInstance context;
    StrategyTestWorkItem item;
    manager.executeWorkflow(&context, &item);
    EXPECT_TRUE(context.getWorkItems().empty());
    EXPECT_EQ(nullptr, item.getResponsible());
}
TEST(WorkflowManagerTests, CancelRemovesRunningInstanceWithoutTakingInstanceOwnership)
{
    TestStaff staff("A", "Doctor");
    WorkflowManager manager;
    RecordingAssignment *assignment = new RecordingAssignment(&staff);
    manager.setAssignmentStrategy(assignment);
    WorkflowInstance context;
    StrategyTestWorkItem item;
    manager.startWorkflow(&context);
    manager.cancelWorkflow(&context);
    EXPECT_NE(nullptr, dynamic_cast<Cancelled *>(context.getState()));
    EXPECT_EQ(&staff, manager.assignWorkItem(&item, &context));
    EXPECT_EQ(0u, assignment->lastRunningCount);
    manager.cancelWorkflow(&context); // repeated cancellation is harmless
    manager.cancelWorkflow(nullptr);
}
TEST(WorkflowManagerTests, FailedRoutingDoesNotRecordOrAssignItem)
{
    TestStaff staff("A", "Doctor");
    WorkflowManager manager;
    manager.setAssignmentStrategy(new RecordingAssignment(&staff));
    manager.setRoutingStrategy(new RecordingRouting(nullptr));
    WorkflowInstance context;
    StrategyTestWorkItem item;
    manager.executeWorkflow(&context, &item);
    EXPECT_EQ(nullptr, item.getResponsible());
    EXPECT_TRUE(context.getWorkItems().empty());
}
TEST(WorkflowManagerTests, ReusingSameStrategyPointerDoesNotDeleteItEarly)
{
    TestStaff staff("A", "Doctor");
    WorkflowManager manager;
    int destroyed = 0;
    RecordingAssignment *strategy = new RecordingAssignment(&staff, &destroyed);
    manager.setAssignmentStrategy(strategy);
    manager.setAssignmentStrategy(strategy);
    EXPECT_EQ(0, destroyed);
    StrategyTestWorkItem item;
    WorkflowInstance context;
    EXPECT_EQ(&staff, manager.assignWorkItem(&item, &context));
}
TEST(WorkflowManagerTests, DestructorDeletesConfiguredStrategiesButNotInstances)
{
    TestStaff staff("A", "Doctor");
    Stage destination;
    int assignmentDestroyed = 0, routingDestroyed = 0;
    WorkflowInstance instance;
    {
        WorkflowManager manager;
        manager.setAssignmentStrategy(new RecordingAssignment(&staff, &assignmentDestroyed));
        manager.setRoutingStrategy(new RecordingRouting(&destination, &routingDestroyed));
        manager.startWorkflow(&instance);
    }
    EXPECT_EQ(1, assignmentDestroyed);
    EXPECT_EQ(1, routingDestroyed);
    EXPECT_NE(nullptr, instance.getState()); // manager did not delete the instance
}

TEST(WorkflowInstanceDataTests, UnsetKeysReadAsTheFallback)
{
    WorkflowInstance context;
    EXPECT_EQ(0.0, context.getData("nothing"));
    EXPECT_EQ(7.5, context.getData("nothing", 7.5));
    context.setData("x", 3.0);
    EXPECT_EQ(3.0, context.getData("x"));
}