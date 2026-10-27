/**
 * @file WorkflowFacadeTest.cpp
 * @brief Behaviour tests for WorkflowFacade: definitions, instance lifecycle,
 *        work submission, invalid input and instance isolation.
 * @author JP
 * @date 2026-10-27
 */

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>

#include "AssignmentStrategy.h"
#include "EventLog.h"
#include "NotificationAdapter.h"
#include "NotificationSystem.h"
#include "RoutingStrategy.h"
#include "SequentialWorkflowBuilder.h"
#include "Stage.h"
#include "Staff.h"
#include "State.h"
#include "Task.h"
#include "WorkflowDefinition.h"
#include "WorkflowDirector.h"
#include "WorkflowEvent.h"
#include "WorkflowFacade.h"
#include "WorkflowInstance.h"

namespace {

/** Staff member with a fixed name, for assignment checks. */
class TestStaff : public Staff {
public:
    std::string getName() { return "Sam"; }
    std::string getTitle() { return "Reviewer"; }
    int getRank() { return 1; }
    bool handleRequest(int) { return false; }
};

/** Always assigns the same staff member. */
class FixedAssignment : public AssignmentStrategy {
public:
    explicit FixedAssignment(Staff* staff) : staff(staff) {}
    Staff* selectAssignmentItem(WorkItem*, WorkflowInstance*, const std::vector<WorkflowInstance*>&) { return staff; }
private:
    Staff* staff;
};

/** Routes every item to the first stage of the instance it belongs to. */
class FirstStageRouting : public RoutingStrategy {
public:
    Stage* selectItemState(WorkItem*, WorkflowInstance* context) override {
        return dynamic_cast<Stage*>(context->getWorkItems().front());
    }
};

/** Builds a two-stage definition the same way the TUI does, through the Director. */
WorkflowDefinition* makeDefinition(const std::string& name) {
    SequentialWorkflowBuilder builder;
    WorkflowDirector director(&builder);
    std::map<std::string, std::vector<std::string>> tasks;
    tasks["Intake"].push_back("Log request");
    director.construct(name, {"Intake", "Review"}, tasks);
    return builder.getWorkflowDefinition();
}

std::string stateOf(WorkflowInstance* instance) {
    return instance->getState()->getName();
}

}  // namespace

// Definitions

TEST(WorkflowFacadeTest, AddDefinitionKeepsOrderAndIgnoresNullAndDuplicates) {
    WorkflowFacade facade;
    WorkflowDefinition* first = makeDefinition("First");
    WorkflowDefinition* second = makeDefinition("Second");

    facade.addDefinition(first);
    facade.addDefinition(nullptr);
    facade.addDefinition(second);
    facade.addDefinition(first);

    std::vector<WorkflowDefinition*> definitions = facade.getDefinitions();
    ASSERT_EQ(2u, definitions.size());
    EXPECT_EQ(first, definitions[0]);
    EXPECT_EQ(second, definitions[1]);
}

// Creating and finding instances

TEST(WorkflowFacadeTest, CreateInstanceStartsPendingWithNumberedIds) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);

    WorkflowInstance* a = facade.createWorkflowInstance(definition);
    WorkflowInstance* b = facade.createWorkflowInstance(definition);

    ASSERT_NE(nullptr, a);
    ASSERT_NE(nullptr, b);
    EXPECT_EQ("WF-1", a->getId());
    EXPECT_EQ("WF-2", b->getId());
    EXPECT_EQ("Pending", stateOf(a));
    ASSERT_EQ(2u, facade.getInstances().size());
    EXPECT_EQ(a, facade.getInstances()[0]);
    EXPECT_EQ(b, facade.getInstances()[1]);
}

TEST(WorkflowFacadeTest, CreateInstanceRejectsNullDefinition) {
    WorkflowFacade facade;
    EXPECT_EQ(nullptr, facade.createWorkflowInstance(nullptr));
    EXPECT_TRUE(facade.getInstances().empty());
}

TEST(WorkflowFacadeTest, InstanceGetsTheStagesOfItsDefinition) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);

    WorkflowInstance* instance = facade.createWorkflowInstance(definition);

    ASSERT_EQ(2u, instance->getWorkItems().size());
    EXPECT_EQ("Intake", instance->getWorkItems()[0]->getName());
    EXPECT_EQ("Review", instance->getWorkItems()[1]->getName());
}

TEST(WorkflowFacadeTest, GetInstanceFindsByIdAndReturnsNullOtherwise) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    WorkflowInstance* a = facade.createWorkflowInstance(definition);
    WorkflowInstance* b = facade.createWorkflowInstance(definition);

    EXPECT_EQ(a, facade.getInstance("WF-1"));
    EXPECT_EQ(b, facade.getInstance("WF-2"));
    EXPECT_EQ(nullptr, facade.getInstance("WF-3"));
    EXPECT_EQ(nullptr, facade.getInstance(""));
}

// Lifecycle

TEST(WorkflowFacadeTest, StartWorkflowCreatesAnActiveInstance) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);

    facade.startWorkflow(definition);

    ASSERT_EQ(1u, facade.getInstances().size());
    EXPECT_EQ("Active", stateOf(facade.getInstances().back()));
}

TEST(WorkflowFacadeTest, StartWorkflowIgnoresNullDefinition) {
    WorkflowFacade facade;
    facade.startWorkflow(nullptr);
    EXPECT_TRUE(facade.getInstances().empty());
}

TEST(WorkflowFacadeTest, ExecuteRunsPendingAndActiveInstancesToCompletion) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    WorkflowInstance* pending = facade.createWorkflowInstance(definition);
    facade.startWorkflow(definition);
    WorkflowInstance* active = facade.getInstances().back();

    facade.executeWorkflow(pending);
    facade.executeWorkflow(active);

    EXPECT_EQ("Completed", stateOf(pending));
    EXPECT_EQ("Completed", stateOf(active));

    facade.executeWorkflow(active);
    EXPECT_EQ("Completed", stateOf(active));
}

TEST(WorkflowFacadeTest, CancelStopsARunningInstanceForGood) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    facade.startWorkflow(definition);
    WorkflowInstance* instance = facade.getInstances().back();

    facade.cancelWorkflow(instance);
    EXPECT_EQ("Cancelled", stateOf(instance));

    facade.executeWorkflow(instance);
    EXPECT_EQ("Cancelled", stateOf(instance));
}

TEST(WorkflowFacadeTest, StateChangesRecordHistory) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    facade.startWorkflow(definition);
    WorkflowInstance* instance = facade.getInstances().back();

    facade.executeWorkflow(instance);

    ASSERT_EQ(2u, instance->getEventLog()->getEntries().size());
    EXPECT_EQ("Pending -> Active [WF-1]", instance->getEventLog()->getEntries()[0]->getDescription());
    EXPECT_EQ("Active -> Completed [WF-1]", instance->getEventLog()->getEntries()[1]->getDescription());
}

// Invalid operations

TEST(WorkflowFacadeTest, IgnoresNullAndInstancesItDidNotCreate) {
    WorkflowFacade facade;
    WorkflowInstance foreign;
    Task* item = new Task("Check", "Check");

    facade.executeWorkflow(nullptr);
    facade.cancelWorkflow(nullptr);
    facade.submitWorkItem(nullptr, item);
    facade.executeWorkflow(&foreign);
    facade.cancelWorkflow(&foreign);
    facade.submitWorkItem(&foreign, item);

    EXPECT_EQ("Pending", stateOf(&foreign));
    EXPECT_TRUE(foreign.getWorkItems().empty());
    delete item;  // never accepted, so the caller still owns it
}

// Submitting work

TEST(WorkflowFacadeTest, SubmitAssignsAndRoutesTheItemIntoItsInstance) {
    TestStaff staff;
    WorkflowFacade facade;
    facade.setAssignmentStrategy(new FixedAssignment(&staff));
    facade.setRoutingStrategy(new FirstStageRouting());
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    facade.startWorkflow(definition);
    WorkflowInstance* instance = facade.getInstances().back();
    Task* item = new Task("Check", "Check");

    facade.submitWorkItem(instance, item);

    EXPECT_EQ(&staff, item->getResponsible());
    ASSERT_EQ(3u, instance->getWorkItems().size());
    EXPECT_EQ(item, instance->getWorkItems().back());
    Stage* intake = dynamic_cast<Stage*>(instance->getWorkItems().front());
    ASSERT_NE(nullptr, intake);
    EXPECT_EQ(item, intake->getMembers().back());
}

TEST(WorkflowFacadeTest, SubmitWithoutStrategiesLeavesTheItemWithTheCaller) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    facade.startWorkflow(definition);
    WorkflowInstance* instance = facade.getInstances().back();
    Task* item = new Task("Check", "Check");

    facade.submitWorkItem(instance, item);

    EXPECT_EQ(2u, instance->getWorkItems().size());
    EXPECT_EQ(nullptr, item->getResponsible());
    delete item;
}

TEST(WorkflowFacadeTest, ExternalSystemHearsAboutSubmittedWork) {
    TestStaff staff;
    NotificationSystem service;
    NotificationAdapter adapter(&service);
    WorkflowFacade facade;
    facade.setExternalSystem(&adapter);
    facade.setAssignmentStrategy(new FixedAssignment(&staff));
    facade.setRoutingStrategy(new FirstStageRouting());
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    facade.startWorkflow(definition);

    facade.submitWorkItem(facade.getInstances().back(), new Task("Check", "Check"));

    EXPECT_EQ(2, service.getSentCount());  // once for assignment, once for routing
}

// Instance isolation

TEST(WorkflowFacadeTest, InstancesOfOneDefinitionDoNotShareStages) {
    WorkflowFacade facade;
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);

    WorkflowInstance* a = facade.createWorkflowInstance(definition);
    WorkflowInstance* b = facade.createWorkflowInstance(definition);

    for (std::size_t i = 0; i < 2; ++i) {
        EXPECT_NE(a->getWorkItems()[i], b->getWorkItems()[i]);
        EXPECT_NE((*definition->getStages())[i], a->getWorkItems()[i]);
    }
}

TEST(WorkflowFacadeTest, ChangingOneInstanceLeavesTheOtherAlone) {
    TestStaff staff;
    WorkflowFacade facade;
    facade.setAssignmentStrategy(new FixedAssignment(&staff));
    facade.setRoutingStrategy(new FirstStageRouting());
    WorkflowDefinition* definition = makeDefinition("Request");
    facade.addDefinition(definition);
    facade.startWorkflow(definition);
    facade.startWorkflow(definition);
    WorkflowInstance* a = facade.getInstances()[0];
    WorkflowInstance* b = facade.getInstances()[1];

    facade.submitWorkItem(a, new Task("Check", "Check"));
    facade.cancelWorkflow(a);

    EXPECT_EQ("Cancelled", stateOf(a));
    EXPECT_EQ("Active", stateOf(b));
    EXPECT_EQ(3u, a->getWorkItems().size());
    EXPECT_EQ(2u, b->getWorkItems().size());
    Stage* intakeB = dynamic_cast<Stage*>(b->getWorkItems().front());
    ASSERT_NE(nullptr, intakeB);
    EXPECT_EQ(1u, intakeB->getMembers().size());  // only its own "Log request" task
    EXPECT_EQ(1u, b->getEventLog()->getEntries().size());
}
