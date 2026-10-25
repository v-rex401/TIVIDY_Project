/**
 * @file EventLogTest.cpp
 * @brief Behaviour tests for the workflow execution history.
 * @author Anke de Frey
 * @date 2026-10-24
 */

#include <gtest/gtest.h>
#include "EventLog.h"
#include "WorkflowEvent.h"

TEST(WorkflowEventTest, StoresWhatItWasGiven) {
    WorkflowEvent e("STATE_CHANGE", "Pending -> Active", "2026-10-24 12:00");
    EXPECT_EQ("STATE_CHANGE", e.getType());
    EXPECT_EQ("Pending -> Active", e.getDescription());
    EXPECT_EQ("2026-10-24 12:00", e.getTimestamp());
}

TEST(EventLogTest, StartsEmpty) {
    EventLog log;
    EXPECT_TRUE(log.getEntries().empty());
}

/** History must be inspectable in the order things happened. */
TEST(EventLogTest, AppendsInOrder) {
    EventLog log;
    log.addEntry(new WorkflowEvent("A", "first", "t1"));
    log.addEntry(new WorkflowEvent("B", "second", "t2"));

    ASSERT_EQ(2u, log.getEntries().size());
    EXPECT_EQ("first", log.getEntries()[0]->getDescription());
    EXPECT_EQ("second", log.getEntries()[1]->getDescription());
}

/** Invalid operation: a null event must not enter the history. */
TEST(EventLogTest, IgnoresNullEntry) {
    EventLog log;
    log.addEntry(NULL);
    EXPECT_TRUE(log.getEntries().empty());
}

/** Instance isolation: one instance's history must not affect another's. */
TEST(EventLogTest, EachLogIsIndependent) {
    EventLog a;
    EventLog b;
    a.addEntry(new WorkflowEvent("A", "only in a", "t1"));

    EXPECT_EQ(1u, a.getEntries().size());
    EXPECT_TRUE(b.getEntries().empty());
}