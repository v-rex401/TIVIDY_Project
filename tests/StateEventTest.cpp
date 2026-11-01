/**
 * @file StateEventTest.cpp
 * @brief Behaviour tests for state transitions, the events they raise, and the
 *        external-system integration point on WorkflowManager.
 * @author Anke de Frey
 * @date 2026-10-26
 */

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "Active.h"
#include "AssignmentStrategy.h"
#include "Cancelled.h"
#include "Clock.h"
#include "Completed.h"
#include "EventLog.h"
#include "ExternalSystem.h"
#include "NotificationAdapter.h"
#include "NotificationSystem.h"
#include "Pending.h"
#include "WorkflowEvent.h"
#include "WorkflowInstance.h"
#include "WorkflowManager.h"
#include "Staff.h"
#include "WorkItem.h"

namespace
{

    /** A minimal work item, so the manager's null guards do not short-circuit. */
    class StubWorkItem : public WorkItem
    {
    public:
        std::string getName() override { return "item"; }
        std::string getTitle() override { return "task"; }
        std::string getSeverity() override { return "Normal"; }
        std::string getRequiredPosition() override { return ""; }
        void setRequiredPosition(const std::string &) override {}
        void printHierarchy(std::string) override {}
        bool handleRequest(int /*rank*/) override { return true; }
        WorkItem *clone() override { return new StubWorkItem(); }
    };

    /** Someone for the strategy to hand the work to. */
    class StubStaff : public Staff
    {
    public:
        std::string getName() override { return "Stub"; }
        std::string getTitle() override { return "Tester"; }
        int getRank() override { return 1; }
        bool handleRequest(int) override { return true; }
    };

    /** Always finds someone, so the work is assigned. */
    class StubAssignment : public AssignmentStrategy
    {
    public:
        Staff *selectAssignmentItem(WorkItem *, WorkflowInstance *,
                                    const std::vector<WorkflowInstance *> &)
        {
            return &who;
        }

    private:
        StubStaff who;
    };

    /** Never finds anyone, so nothing is assigned. */
    class RefusingAssignment : public AssignmentStrategy
    {
    public:
        Staff *selectAssignmentItem(WorkItem *, WorkflowInstance *,
                                    const std::vector<WorkflowInstance *> &)
        {
            return NULL;
        }
    };

}

// Clock

TEST(ClockTest, ProducesATimestampOfTheExpectedShape)
{
    const std::string stamp = Clock::now();
    EXPECT_EQ(19u, stamp.size()); // YYYY-MM-DD HH:MM:SS
    EXPECT_EQ('-', stamp[4]);
    EXPECT_EQ(' ', stamp[10]);
    EXPECT_EQ(':', stamp[13]);
}

// state names

TEST(StateNameTest, EachConcreteStateKnowsItsName)
{
    Pending pending;
    Active active;
    Completed completed;
    Cancelled cancelled;

    EXPECT_EQ("Pending", pending.getName());
    EXPECT_EQ("Active", active.getName());
    EXPECT_EQ("Completed", completed.getName());
    EXPECT_EQ("Cancelled", cancelled.getName());
}

// transitions

TEST(StateTransitionTest, StartsPendingWithNoHistory)
{
    WorkflowInstance wi;
    ASSERT_TRUE(wi.getState() != NULL);
    EXPECT_EQ("Pending", wi.getState()->getName());
    EXPECT_TRUE(wi.getEventLog()->getEntries().empty());
}

TEST(StateTransitionTest, PendingBecomesActiveThenCompleted)
{
    WorkflowInstance wi;
    wi.handle();
    EXPECT_EQ("Active", wi.getState()->getName());
    wi.handle();
    EXPECT_EQ("Completed", wi.getState()->getName());
}

/** Invalid operation: an end state must not transition or log. */
TEST(StateTransitionTest, CompletedRefusesToMove)
{
    WorkflowInstance wi;
    wi.handle();
    wi.handle();
    const size_t before = wi.getEventLog()->getEntries().size();

    wi.handle();

    EXPECT_EQ("Completed", wi.getState()->getName());
    EXPECT_EQ(before, wi.getEventLog()->getEntries().size());
}

TEST(StateTransitionTest, CancelledRefusesToMove)
{
    WorkflowInstance wi;
    wi.setState(new Cancelled());
    wi.handle();
    EXPECT_EQ("Cancelled", wi.getState()->getName());
}

/** Invalid operation: a null state is ignored and changes nothing. */
TEST(StateTransitionTest, NullStateIsIgnored)
{
    WorkflowInstance wi;
    wi.setState(NULL);
    EXPECT_EQ("Pending", wi.getState()->getName());
    EXPECT_TRUE(wi.getEventLog()->getEntries().empty());
}

// event firing

TEST(StateEventTest, EveryTransitionNamesWhereItCameFromAndWentTo)
{
    WorkflowInstance wi;
    wi.handle();
    wi.handle();

    const std::vector<WorkflowEvent *> &log = wi.getEventLog()->getEntries();
    ASSERT_EQ(2u, log.size());
    EXPECT_EQ(0u, log[0]->getDescription().find("Pending -> Active"));
    EXPECT_EQ(0u, log[1]->getDescription().find("Active -> Completed"));
}

TEST(StateEventTest, EventsAreTypedAndTimestamped)
{
    WorkflowInstance wi;
    wi.handle();

    ASSERT_EQ(1u, wi.getEventLog()->getEntries().size());
    EXPECT_EQ("STATE_CHANGE", wi.getEventLog()->getEntries()[0]->getType());
    EXPECT_EQ(19u, wi.getEventLog()->getEntries()[0]->getTimestamp().size());
}

TEST(StateEventTest, TheInstanceIdAppearsInItsHistory)
{
    WorkflowInstance wi;
    wi.setId("WF-1");
    wi.handle();

    ASSERT_EQ(1u, wi.getEventLog()->getEntries().size());
    EXPECT_NE(std::string::npos, wi.getEventLog()->getEntries()[0]->getDescription().find("WF-1"));
}

/** Instance isolation: one instance's transitions must not affect another. */
TEST(StateEventTest, EachInstanceHasItsOwnHistory)
{
    WorkflowInstance a;
    WorkflowInstance b;
    a.handle();

    EXPECT_EQ(1u, a.getEventLog()->getEntries().size());
    EXPECT_TRUE(b.getEventLog()->getEntries().empty());
    EXPECT_EQ("Pending", b.getState()->getName());
}

// Adapter through the manager

/** The engine reaches the concrete service only through ExternalSystem. */
TEST(ManagerAdapterTest, AssigningWorkNotifiesThroughTheAdapter)
{
    NotificationSystem service;
    NotificationAdapter adapter(&service);

    WorkflowManager manager;
    manager.setAssignmentStrategy(new StubAssignment()); // manager deletes it
    manager.setExternalSystem(&adapter);

    StubWorkItem item;
    WorkflowInstance context;

    manager.assignWorkItem(&item, &context);
    EXPECT_EQ(1, service.getSentCount());

    manager.assignWorkItem(&item, &context);
    EXPECT_EQ(2, service.getSentCount());
}

/** Nothing was assigned, so there is nothing to tell the outside system. */
TEST(ManagerAdapterTest, AFailedAssignmentNotifiesNothing)
{
    NotificationSystem service;
    NotificationAdapter adapter(&service);

    WorkflowManager manager;
    manager.setAssignmentStrategy(new RefusingAssignment()); // manager deletes it
    manager.setExternalSystem(&adapter);

    StubWorkItem item;
    WorkflowInstance context;

    EXPECT_TRUE(manager.assignWorkItem(&item, &context) == NULL);
    EXPECT_EQ(0, service.getSentCount());
}

/** Without an external system the engine still runs. */
TEST(ManagerAdapterTest, RunsWithoutAnExternalSystem)
{
    WorkflowManager manager;
    manager.setAssignmentStrategy(new StubAssignment());
    manager.setExternalSystem(NULL);

    StubWorkItem item;
    WorkflowInstance context;
    EXPECT_NO_THROW(manager.assignWorkItem(&item, &context));
}

/** Invalid operation: no strategy configured must not crash. */
TEST(ManagerAdapterTest, ReturnsNullWithoutAStrategy)
{
    WorkflowManager manager;
    EXPECT_TRUE(manager.assignWorkItem(NULL, NULL) == NULL);
    EXPECT_TRUE(manager.routeWorkItem(NULL, NULL) == NULL);
}
