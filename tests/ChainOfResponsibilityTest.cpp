/**
 * @file ChainOfResponsibilityTest.cpp
 * @brief Unit tests for Chain Of Responsibility pattern
 * @author Kayla Falconer
 * @date 2026-10-27
 */

#include <gtest/gtest.h>

#include "Employee.h"
#include "Team.h"
#include "Task.h"
#include "Handler.h"

/**
 * @brief Check an employee handles a request with sufficient rank
 */
TEST(ChainOfResponsibilityTest, EmployeeHandlesRequest)
{
    Employee employee("Alice", "Doctor", 5);

    EXPECT_TRUE(employee.handleRequest(3));
}

/**
 * @brief Checks an employee cannot handle a request with insufficient rank
 */
TEST(ChainOfResponsibilityTest, EmployeeRejectsRequest)
{
    Employee employee("Alice", "Doctor", 2);

    EXPECT_FALSE(employee.handleRequest(5));
}

/**
 * @brief Checks that an employee forwards request to successor
 */
TEST(ChainOfResponsibilityTest, EmployeeEscalatesRequest)
{
    Employee junior("Alice", "Junior Doctor", 2);
    Employee senior("Bob", "Senior Doctor", 5);

    junior.setNextHandler(&senior);

    EXPECT_TRUE(junior.handleRequest(4));
}

/**
 * @brief Checks that a team forwards to another Staff handler
 */
TEST(ChainOfResponsibilityTest, TeamEscalatesRequest)
{
    Team team("Junior Team", "Doctors", 2);
    Employee senior("Bob", "Senior Doctor", 5);

    team.setNextHandler(&senior);

    EXPECT_TRUE(team.handleRequest(4));
}

/**
 * @brief Checks that a task delegates to responsible Staff
 */
TEST(ChainOfResponsibilityTest, TaskDelegatesToStaff)
{
    Employee employee("Alice", "Doctor", 5);
    Task task("T1", "Review Patient");

    task.setResponsible(&employee);

    EXPECT_TRUE(task.handleRequest(3));
}

/**
 * @brief Checks that a task forwards to another WorkItem
 */
TEST(ChainOfResponsibilityTest, TaskEscalatesToNextTask)
{
    Employee junior("Alice", "Junior Doctor", 2);
    Employee senior("Bob", "Senior Doctor", 5);

    Task first("T1", "Initial Review");
    Task second("T2", "Senior Review");

    first.setResponsible(&junior);
    second.setResponsible(&senior);

    first.setNextHandler(&second);

    EXPECT_TRUE(first.handleRequest(4));
}

/**
 * @brief Checks that a handler delegates to Staff
 */
TEST(ChainOfResponsibilityTest, HandlerDelegatesToStaff)
{
    Employee employee("Alice", "Doctor", 5);
    Handler handler("H1", "Approval Handler");

    handler.setResponsible(&employee);

    EXPECT_TRUE(handler.handleRequest(4));
}

/**
 * @brief Checks that a handler forwards to next WorkItem
 */
TEST(ChainOfResponsibilityTest, HandlerEscalatesRequest)
{
    Employee senior("Bob", "Senior Doctor", 5);

    Handler first("H1", "Initial Approval");
    Handler second("H2", "Senior Approval");

    second.setResponsible(&senior);
    first.setNextHandler(&second);

    EXPECT_TRUE(first.handleRequest(4));
}

/**
 * @brief Checks that a request fails when no handler can process it
 */
TEST(ChainOfResponsibilityTest, UnhandledRequest)
{
    Employee junior("Alice", "Junior Doctor", 2);

    Task task("T1", "Review");
    task.setResponsible(&junior);

    EXPECT_FALSE(task.handleRequest(5));
}
