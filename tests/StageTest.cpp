/**
 * @file StageTest.cpp
 * @brief Unit tests for the Stage class in the Composite pattern
 * @author Kayla Falconer
 * @date 2024-10-24
 */

#include <gtest/gtest.h>
#include <string>

#include "Stage.h"
#include "Task.h"

/**
 * @brief Tests that adding a member correctly updates the Stage hierarchy
 */
TEST(CompositeTest, AddMember)
{
    Stage stage;
    Task task;

    stage.addMember(new Task()); //the stage owns it

    testing::internal::CaptureStdout();
    stage.printHierarchy("");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Task:"), std::string::npos);
}

/**
 * @brief Tests that removing a member correctly updates the Stage hierarchy
 */
TEST(CompositeTest, RemoveMember)
{
    Stage stage;
    Task* task = new Task();

    stage.addMember(task);
    stage.removeMember(task); //detached, so this test owns it again

    testing::internal::CaptureStdout();
    stage.printHierarchy("");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find("Task:"), std::string::npos);

    delete task;
}

/**
 * @brief Test that a null member will not be added to the Stage
 */
TEST(CompositeTest, AddNullMember)
{
    Stage stage;

    EXPECT_NO_THROW(stage.addMember(nullptr));
}

/**
 * @brief Test for printing the hierarchy of the Stage
 */
TEST(CompositeTest, PrintHierarchy)
{
    Stage stage;

    testing::internal::CaptureStdout();
    stage.printHierarchy("");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Stage"), std::string::npos);
}