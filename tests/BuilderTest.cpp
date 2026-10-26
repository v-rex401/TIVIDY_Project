/**
 * @file BuilderTest.cpp
 * @brief Unit tests for Builder pattern
 * @author Kayla Falconer
 * @date 2026-10-26
 */

 #include <gtest/gtest.h>
 #include <string>
 #include <vector>
 #include <map>

 #include "Stage.h"
 #include "WorkflowDirector.h"
 #include "WorkflowBuilder.h"
 #include "SequentialWorkflowBuilder.h"
 #include "EscalatingWorkflowBuilder.h"
 #include "WorkflowDefinition.h"
 #include "WorkItem.h"

/**
 * @brief Tests that the Director uses the SequentialWorkflowBuilder to construct a workflow with the correct number and order of stages
 */
TEST(BuilderTest, SequentialWorkflowCreation)
{
    SequentialWorkflowBuilder builder;
    WorkflowDirector director(&builder);

    std::vector<std::string> stages = {"Registration", "Assessment", "Approval"};

    std::map<std::string, std::vector<std::string>> tasks = {{"Registration", {"Capture details"}}, {"Assessment", {"Review application"}}, {"Approval", {"Approve application"}}};

    director.construct("Test Workflow", stages, tasks);

    WorkflowDefinition* workflow = builder.getWorkflowDefinition();

    ASSERT_NE(workflow, nullptr);
    ASSERT_EQ(workflow->getStages()->size(), 3u);

    EXPECT_EQ(workflow->getStages()->at(0)->getName(), "Registration");
    EXPECT_EQ(workflow->getStages()->at(1)->getName(), "Assessment");
    EXPECT_EQ(workflow->getStages()->at(2)->getName(), "Approval");

    delete workflow;
}

/**
 * @brief Tests that the Director uses the EscalatingWorkflowBuilder to construct a workflow with the correct stages
 */
TEST(BuilderTest, EscalatingWorkflowCreation)
{
    EscalatingWorkflowBuilder builder;
    WorkflowDirector director(&builder);

    std::vector<std::string> stages = {"Senior Doctor", "Doctor", "Nurse"};

    std::map<std::string, std::vector<std::string>> tasks = {{"Senior Doctor", {"Review complex case"}}, {"Doctor", {"Assess patient"}}, {"Nurse", {"Record observations"}}};

    director.construct("Escalating Workflow", stages, tasks);

    WorkflowDefinition* workflow = builder.getWorkflowDefinition();

    ASSERT_NE(workflow, nullptr);
    ASSERT_EQ(workflow->getStages()->size(), 3u);

    EXPECT_EQ(workflow->getStages()->at(0)->getName(), "Senior Doctor");
    EXPECT_EQ(workflow->getStages()->at(1)->getName(), "Doctor");
    EXPECT_EQ(workflow->getStages()->at(2)->getName(), "Nurse");

    delete workflow;
}

/**
 * @brief Tests that the Director can construct a workflow containing no stages / tasks
 */
TEST(BuilderTest, EmptyWorkflow)
{
    SequentialWorkflowBuilder builder;
    WorkflowDirector director(&builder);

    //empty stages + tasks
    std::vector<std::string> stages;
    std::map<std::string, std::vector<std::string>> tasks;

    director.construct("Empty Workflow", stages, tasks);

    WorkflowDefinition* workflow = builder.getWorkflowDefinition();

    ASSERT_NE(workflow, nullptr);
    EXPECT_TRUE(workflow->getStages()->empty());

    delete workflow;
}

/**
 * @brief Tests that getWorkflowDefinition() transfers ownership of the constructed workflow + resets the Builder's pointer
 */
TEST(BuilderTest, GetWorkflowDefinition)
{
    SequentialWorkflowBuilder builder;

    builder.buildDefinition("Test Workflow");
    builder.buildStage("Registration");

    WorkflowDefinition* workflow = builder.getWorkflowDefinition();

    ASSERT_NE(workflow, nullptr);
    EXPECT_EQ(workflow->getStages()->size(), 1u);

    //builder should no longer own the returned product
    EXPECT_EQ(builder.getWorkflowDefinition(), nullptr);

    delete workflow;
}

/**
 * @brief Tests that tasks are added to their corresponding stages + don't appear in other stages
 */
TEST(BuilderTest, TasksAddedToCorrectStages)
{
    SequentialWorkflowBuilder builder;
    WorkflowDirector director(&builder);

    std::vector<std::string> stages = {"Registration", "Approval"};

    std::map<std::string, std::vector<std::string>> tasks = {{"Registration", {"Capture details"}}, {"Approval", {"Approve application"}}};

    director.construct("Test Workflow", stages, tasks);

    WorkflowDefinition* workflow = builder.getWorkflowDefinition();

    ASSERT_NE(workflow, nullptr);
    ASSERT_EQ(workflow->getStages()->size(), 2u);

    Stage* registration = dynamic_cast<Stage*>(workflow->getStages()->at(0));

    Stage* approval = dynamic_cast<Stage*>(workflow->getStages()->at(1));

    ASSERT_NE(registration, nullptr);
    ASSERT_NE(approval, nullptr);

    testing::internal::CaptureStdout();
    registration->printHierarchy("");
    std::string registrationOutput = testing::internal::GetCapturedStdout();

    testing::internal::CaptureStdout();
    approval->printHierarchy("");
    std::string approvalOutput = testing::internal::GetCapturedStdout();

    EXPECT_NE(registrationOutput.find("Capture details"), std::string::npos);

    EXPECT_NE(approvalOutput.find("Approve application"), std::string::npos);

    //check that tasks were not placed in the wrong stage
    EXPECT_EQ(registrationOutput.find("Approve application"), std::string::npos);

    EXPECT_EQ(approvalOutput.find("Capture details"), std::string::npos);

    delete workflow;
}