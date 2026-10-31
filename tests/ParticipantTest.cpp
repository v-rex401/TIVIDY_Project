/**
 * @file ParticipantTest.cpp
 * @brief Tests that an instance records the staff who work on it.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include "Employee.h"
#include "RoleBasedAssignment.h"
#include "SeverityBasedRouting.h"
#include "Stage.h"
#include "Task.h"
#include "WorkflowDefinition.h"
#include "WorkflowFacade.h"
#include "WorkflowInstance.h"

TEST(ParticipantTest, NewInstanceHasNoParticipants)
{
    WorkflowInstance instance;
    EXPECT_TRUE(instance.getParticipants().empty());
}

TEST(ParticipantTest, AddParticipantIgnoresNullAndDuplicates)
{
    WorkflowInstance instance;
    Employee jane("Jane", "Nurse", 2);
    instance.addParticipant(&jane);
    instance.addParticipant(&jane);
    instance.addParticipant(nullptr);

    ASSERT_EQ(instance.getParticipants().size(), 1u);
    EXPECT_EQ(instance.getParticipants()[0], &jane);
}

namespace
{
    /**
     * @brief A facade with one "Ward" stage, role-based assignment and one nurse.
     */
    struct OneWard
    {
        Employee nurse{"Jane", "Nurse", 2}; ///< The only person on the roster.
        WorkflowFacade facade;              ///< Owns the definition and instances.
        WorkflowDefinition *definition;     ///< Registered with the facade.

        OneWard() : definition(new WorkflowDefinition())
        {
            definition->addStage(new Stage("Ward"));
            facade.addDefinition(definition);
            facade.setAssignmentStrategy(new RoleBasedAssignment({&nurse}));
            facade.setRoutingStrategy(new SeverityBasedRouting("Ward", "Ward"));
        }
    };
}

TEST(ParticipantTest, SubmittedWorkAddsTheAssigneeAsParticipant)
{
    OneWard ward;
    WorkflowInstance *instance = ward.facade.createWorkflowInstance(ward.definition);
    Task *task = new Task("Check vitals");
    task->setRequiredPosition("Nurse");
    ward.facade.submitWorkItem(instance, task);

    ASSERT_EQ(instance->getParticipants().size(), 1u);
    EXPECT_EQ(instance->getParticipants()[0], &ward.nurse);
}

TEST(ParticipantTest, WorkNobodyCanTakeAddsNoParticipant)
{
    OneWard ward;
    WorkflowInstance *instance = ward.facade.createWorkflowInstance(ward.definition);
    Task task("Operate");
    task.setRequiredPosition("Surgeon");
    ward.facade.submitWorkItem(instance, &task);

    EXPECT_TRUE(instance->getParticipants().empty());
}

TEST(ParticipantTest, ParticipantsStayWithTheirOwnInstance)
{
    OneWard ward;
    WorkflowInstance *first = ward.facade.createWorkflowInstance(ward.definition);
    WorkflowInstance *second = ward.facade.createWorkflowInstance(ward.definition);
    Task *task = new Task("Check vitals");
    task->setRequiredPosition("Nurse");
    ward.facade.submitWorkItem(first, task);

    EXPECT_EQ(first->getParticipants().size(), 1u);
    EXPECT_TRUE(second->getParticipants().empty());
}
