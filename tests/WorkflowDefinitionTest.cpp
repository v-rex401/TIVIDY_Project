/**
 * @file WorkflowDefinitionTest.cpp
 * @brief Tests for what a built WorkflowDefinition reports about itself and how it clones.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include <map>
#include <string>
#include <vector>

#include "EscalatingWorkflowBuilder.h"
#include "SequentialWorkflowBuilder.h"
#include "WorkflowDefinition.h"
#include "WorkflowDirector.h"

namespace
{
    /**
     * @brief Build a three-stage definition with the given builder.
     * @param builder The concrete builder to direct.
     * @return The product; the caller owns it.
     */
    WorkflowDefinition *buildThreeStages(WorkflowBuilder &builder)
    {
        WorkflowDirector director(&builder);
        std::vector<std::string> stages = {"Intake", "Review", "Close"};
        std::map<std::string, std::vector<std::string>> tasks = {{"Intake", {"Capture"}}};
        director.construct("Claims", stages, tasks);
        builder.setAssignmentStrategyType("RoleBased");
        builder.setRoutingStrategyType("EventBased");
        return builder.getWorkflowDefinition();
    }
}

TEST(WorkflowDefinitionTest, ReportsNameAndStrategyTypes)
{
    SequentialWorkflowBuilder builder;
    WorkflowDefinition *definition = buildThreeStages(builder);

    EXPECT_EQ(definition->getName(), "Claims");
    EXPECT_EQ(definition->getAssignmentStrategyType(), "RoleBased");
    EXPECT_EQ(definition->getRoutingStrategyType(), "EventBased");
    delete definition;
}

TEST(WorkflowDefinitionTest, EmptyDefinitionHasNoStrategyTypes)
{
    WorkflowDefinition definition;
    EXPECT_EQ(definition.getName(), "");
    EXPECT_EQ(definition.getAssignmentStrategyType(), "");
    EXPECT_EQ(definition.getRoutingStrategyType(), "");
    EXPECT_TRUE(definition.getTransitions().empty());
}

TEST(WorkflowDefinitionTest, TransitionsFollowTheStageOrder)
{
    SequentialWorkflowBuilder builder;
    WorkflowDefinition *definition = buildThreeStages(builder);

    std::vector<std::pair<std::string, std::string>> transitions = definition->getTransitions();
    ASSERT_EQ(transitions.size(), 2u);
    EXPECT_EQ(transitions[0], std::make_pair(std::string("Intake"), std::string("Review")));
    EXPECT_EQ(transitions[1], std::make_pair(std::string("Review"), std::string("Close")));
    delete definition;
}

TEST(WorkflowDefinitionTest, CloneCopiesNameAndStrategyTypes)
{
    SequentialWorkflowBuilder builder;
    WorkflowDefinition *definition = buildThreeStages(builder);
    WorkflowDefinition *copy = definition->clone();

    EXPECT_EQ(copy->getName(), "Claims");
    EXPECT_EQ(copy->getRoutingStrategyType(), "EventBased");
    EXPECT_EQ(copy->getTransitions(), definition->getTransitions());
    delete copy;
    delete definition;
}

TEST(WorkflowDefinitionTest, CloneKeepsTheSequentialHandlerLinks)
{
    SequentialWorkflowBuilder builder;
    WorkflowDefinition *definition = buildThreeStages(builder);
    WorkflowDefinition *copy = definition->clone();

    Stage *intake = copy->findStage("Intake");
    Stage *review = copy->findStage("Review");
    Stage *close = copy->findStage("Close");
    EXPECT_EQ(intake->getNextHandler(), review);
    EXPECT_EQ(review->getNextHandler(), close);
    EXPECT_EQ(close->getNextHandler(), nullptr);
    delete copy;
    delete definition;
}

TEST(WorkflowDefinitionTest, CloneKeepsTheEscalatingHandlerLinks)
{
    EscalatingWorkflowBuilder builder;
    WorkflowDefinition *definition = buildThreeStages(builder);
    WorkflowDefinition *copy = definition->clone();

    EXPECT_EQ(copy->findStage("Close")->getNextHandler(), copy->findStage("Review"));
    EXPECT_EQ(copy->findStage("Review")->getNextHandler(), copy->findStage("Intake"));
    EXPECT_EQ(copy->findStage("Intake")->getNextHandler(), nullptr);
    delete copy;
    delete definition;
}

TEST(WorkflowDefinitionTest, CloneLinksPointIntoTheCopyNotTheOriginal)
{
    SequentialWorkflowBuilder builder;
    WorkflowDefinition *definition = buildThreeStages(builder);
    WorkflowDefinition *copy = definition->clone();

    EXPECT_NE(copy->findStage("Intake")->getNextHandler(), definition->findStage("Review"));
    delete copy;
    delete definition;
}
