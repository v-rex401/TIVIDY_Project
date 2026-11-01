/**
 * @file SeverityRoutingTest.cpp
 * @brief Tests that a work item's severity decides where SeverityBasedRouting sends it.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include "SeverityBasedRouting.h"
#include "Stage.h"
#include "Task.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"

namespace
{
    /**
     * @brief An instance whose definition has a "Normal" and an "Urgent" stage.
     *
     * The instance does not own its definition, so the fixture keeps both.
     */
    struct TwoStageCase
    {
        WorkflowDefinition definition; ///< Holds the two stages.
        WorkflowInstance instance;     ///< Points at the definition.

        TwoStageCase()
        {
            definition.addStage(new Stage("Normal"));
            definition.addStage(new Stage("Urgent"));
            instance.setDefinition(&definition);
        }
    };
}

TEST(SeverityRoutingTest, TaskStartsWithNoSeverity)
{
    Task task("Check vitals");
    EXPECT_EQ(task.getSeverity(), "");
}

TEST(SeverityRoutingTest, SetSeverityIsKeptAndCloned)
{
    Task task("Check vitals");
    task.setSeverity("High");
    WorkItem *copy = task.clone();

    EXPECT_EQ(task.getSeverity(), "High");
    EXPECT_EQ(copy->getSeverity(), "High");
    delete copy;
}

TEST(SeverityRoutingTest, HighOrCriticalGoesToTheEmergencyStage)
{
    TwoStageCase c;
    SeverityBasedRouting routing("Normal", "Urgent");
    Task high("Stabilise");
    Task critical("Resuscitate");
    high.setSeverity("High");
    critical.setSeverity("Critical");

    EXPECT_EQ(routing.selectItemState(&high, &c.instance), c.definition.findStage("Urgent"));
    EXPECT_EQ(routing.selectItemState(&critical, &c.instance), c.definition.findStage("Urgent"));
}

TEST(SeverityRoutingTest, LowOrUnsetGoesToTheNormalStage)
{
    TwoStageCase c;
    SeverityBasedRouting routing("Normal", "Urgent");
    Task low("Register");
    Task unset("File report");
    low.setSeverity("Low");

    EXPECT_EQ(routing.selectItemState(&low, &c.instance), c.definition.findStage("Normal"));
    EXPECT_EQ(routing.selectItemState(&unset, &c.instance), c.definition.findStage("Normal"));
}

TEST(SeverityRoutingTest, StageSeverityCanBeSet)
{
    Stage stage("Emergency Care");
    stage.setSeverity("Critical");
    EXPECT_EQ(stage.getSeverity(), "Critical");
}
