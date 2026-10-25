/**
 * @file ObserverTests.cpp
 * @brief Behaviour tests for the Observer pattern participants.
 */
#include <gtest/gtest.h>
#include <string>
#include "AuditLogObserver.h"
#include "EventLog.h"
#include "Observer.h"
#include "ProgressMonitor.h"
#include "WorkflowEvent.h"
#include "WorkflowInstance.h"
/** Test observer: counts updates and remembers the last event. */
class SpyObserver : public Observer {
public:
    SpyObserver() : calls(0), lastEvent(NULL) {}
    void update(WorkflowEvent* event, WorkflowInstance*) override {
        ++calls;
        lastEvent = event;
    }
    int calls;
    WorkflowEvent* lastEvent;
};
 
/** Adds an event to the instance's log (the log owns it). */
static WorkflowEvent* logEvent(WorkflowInstance& instance) {
    WorkflowEvent* event = new WorkflowEvent("STATE", "started", "10:00");
    instance.getEventLog()->addEntry(event);
    return event;
}
 
TEST(WorkflowSubjectTest, AttachThenNotifyDeliversTheEvent) {
    WorkflowInstance instance;
    SpyObserver observer;
    instance.attach(&observer);
    WorkflowEvent* event = logEvent(instance);
    instance.notify();
    EXPECT_EQ(1, observer.calls);
    EXPECT_EQ(event, observer.lastEvent);
}
 
TEST(WorkflowSubjectTest, DetachThenNotifyDoesNotDeliver) {
    WorkflowInstance instance;
    SpyObserver observer;
    instance.attach(&observer);
    instance.detach(&observer);
    logEvent(instance);
    instance.notify();
    EXPECT_EQ(0, observer.calls);
}
 
TEST(WorkflowSubjectTest, AttachingTheSameObserverTwiceNotifiesItOnce) {
    WorkflowInstance instance;
    SpyObserver observer;
    instance.attach(&observer);
    instance.attach(&observer);
    logEvent(instance);
    instance.notify();
    EXPECT_EQ(1, observer.calls);
}
 
TEST(WorkflowSubjectTest, NotifyWithNoObserversDoesNotCrash) {
    WorkflowInstance instance;
    logEvent(instance);
    EXPECT_NO_THROW(instance.notify());
}
 
TEST(AuditLogObserverTest, UpdateRecordsTheEventInTheInstancesLog) {
    WorkflowInstance instance;
    AuditLogObserver audit;
    WorkflowEvent* event = new WorkflowEvent("APPROVE", "approved", "10:05");
    audit.update(event, &instance);
    ASSERT_EQ(1u, instance.getEventLog()->getEntries().size());
    EXPECT_EQ(event, instance.getEventLog()->getEntries()[0]);
}
 
TEST(AuditLogObserverTest, AnEventAlreadyInTheLogIsNotAppendedAgain) {
    WorkflowInstance instance;
    AuditLogObserver audit;
    instance.attach(&audit);
    logEvent(instance);
    instance.notify();
    EXPECT_EQ(1u, instance.getEventLog()->getEntries().size());
}
 
TEST(ProgressMonitorTest, UpdateReportsTheEvent) {
    WorkflowInstance instance;
    ProgressMonitor monitor;
    WorkflowEvent event("STATE", "Pending->Active", "10:00");
    testing::internal::CaptureStdout();
    monitor.update(&event, &instance);
    EXPECT_EQ("[PROGRESS] [10:00] STATE: Pending->Active\n", testing::internal::GetCapturedStdout());
}
 
TEST(ObserverTest, NullArgumentsAreSafe) {
    WorkflowInstance instance;
    AuditLogObserver audit;
    ProgressMonitor monitor;
    EXPECT_NO_THROW(audit.update(NULL, &instance));
    EXPECT_NO_THROW(monitor.update(NULL, NULL));
}
