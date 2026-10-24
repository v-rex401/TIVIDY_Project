/**
 * @file PrototypeTest.cpp
 * @brief Unit tests for Prototype pattern
 * @author Kayla Falconer
 * @date 2026-10-24
 */

 #include <gtest/gtest.h>
 #include <string>

 #include "WorkItem.h"
 #include "Stage.h"
 #include "Task.h"

 /**
  * @brief Tests that cloning a Stage object creates a new object
  */
 TEST(PrototypeTest, CloneStage)
{
    Stage stage;

    WorkItem* clonedStage = stage.clone();


    ASSERT_NE(clonedStage, nullptr);
    EXPECT_NE(clonedStage, &stage);

    delete clonedStage;
}

/**
 * @brief Tests that cloning a Task object creates a new object
 */
TEST(PrototypeTest, CloneTask)
{
    Task task;

    WorkItem* clonedTask = task.clone();


    ASSERT_NE(clonedTask, nullptr);
    EXPECT_NE(clonedTask, &task);

    delete clonedTask;
}

/**
 * @brief Tests that a cloned Stage preserves its original attributes
 */
TEST(PrototypeTest, CloneStageAttributes)
{
    Stage stage;

    WorkItem* clonedStage = stage.clone();


    ASSERT_NE(clonedStage, nullptr);

    EXPECT_EQ(clonedStage->getName(), stage.getName());
    EXPECT_EQ(clonedStage->getTitle(), stage.getTitle());

    delete clonedStage;
}

/**
 * @brief Tests that a cloned Task preserves its original attributes
 */
TEST(PrototypeTest, CloneTaskAttributes)
{
    Task task;

    WorkItem* clonedTask = task.clone();

    
    ASSERT_NE(clonedTask, nullptr);

    EXPECT_EQ(clonedTask->getName(), task.getName());
    EXPECT_EQ(clonedTask->getTitle(), task.getTitle());

    delete clonedTask;
}
