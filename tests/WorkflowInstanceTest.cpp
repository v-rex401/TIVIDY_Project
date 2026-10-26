/**
 * @file WorkflowInstanceTest.cpp
 * @brief Behaviour tests for the State, Memento, and Observer patterns in WorkflowInstance.
 * @author Jezelle Govender
 * @date 2026-10-25
 */

#include <gtest/gtest.h>
#include "WorkflowInstance.h"
#include "Pending.h"
#include "Active.h"
#include "Completed.h"
#include "Observer.h"
#include "WorkflowEvent.h"
#include "EventLog.h"

// A simple mock observer to test the Observer pattern
class MockObserver : public Observer {
public:
    int updateCount = 0;
    
    void update(WorkflowEvent* event, WorkflowInstance* instance) override {
        updateCount++;
    }
};

// State Pattern Test
TEST(WorkflowInstanceTest, StartsInPendingState) {
    WorkflowInstance instance;
    
    // Check that the initial state is Pending
    EXPECT_NE(nullptr, dynamic_cast<Pending*>(instance.getState()));
}

TEST(WorkflowInstanceTest, TransitionsFromPendingToActive) {
    WorkflowInstance instance;
    
    // Handle the workflow (triggers Pending -> Active)
    instance.handle();
    
    // Verify it is now Active
    EXPECT_NE(nullptr, dynamic_cast<Active*>(instance.getState()));
}

TEST(WorkflowInstanceTest, TransitionsToCompleted) {
    WorkflowInstance instance;
    
    instance.handle(); // Pending -> Active
    instance.handle(); // Active -> Completed
    
    EXPECT_NE(nullptr, dynamic_cast<Completed*>(instance.getState()));  // Verify it is now Completed
}

//Memento Pattern Tests
TEST(WorkflowInstanceTest, MementoRestoresPreviousState) {
    WorkflowInstance instance;
    
    // 1. Save the initial state (Pending)
    Memento* savedState = instance.createMemento();
    EXPECT_NE(nullptr, dynamic_cast<Pending*>(instance.getState()));
    
    // 2. Change the state to Active
    instance.handle();
    EXPECT_NE(nullptr, dynamic_cast<Active*>(instance.getState()));
    
    // 3. Restore the saved state (back to Pending)
    instance.setMemento(savedState);
    
    // 4. Verify it is back to Pending
    EXPECT_NE(nullptr, dynamic_cast<Pending*>(instance.getState()));
    
    delete savedState;
}

TEST(WorkflowInstanceTest, MementoRestoreIsLoggedOnce) {
    WorkflowInstance instance;
    Memento* saved = instance.createMemento();
    instance.handle();                       // 1 entry
    instance.setMemento(saved);              // should make it 2

    EXPECT_EQ(2u, instance.getEventLog()->getEntries().size());
    delete saved;
}


// Observer Pattern Test-
TEST(WorkflowInstanceTest, NotifiesObserversOnStateChange) {
    WorkflowInstance instance;
    MockObserver observer;
    
    // Attach the observer
    instance.attach(&observer);
    
    // No changes yet, so updateCount should be 0
    EXPECT_EQ(0, observer.updateCount);
    
    // Trigger a state change (Pending -> Active)
    instance.handle();
    
    // The observer should have been notified once
    EXPECT_EQ(1, observer.updateCount);
}

TEST(WorkflowInstanceTest, DetachesObservers) {
    WorkflowInstance instance;
    MockObserver observer;
    
    instance.attach(&observer);
    instance.detach(&observer);
    
    // Trigger a state change
    instance.handle();
    
    // The observer was detached, so it should not be notified
    EXPECT_EQ(0, observer.updateCount);
}

TEST(WorkflowInstanceTest, ObserverIsNotAttachedTwice) {
    WorkflowInstance instance;
    MockObserver observer;
    
    // Attach twice
    instance.attach(&observer);
    instance.attach(&observer);
    
    // Trigger one state change
    instance.handle();
    
    // Should only be notified once, not twice
    EXPECT_EQ(1, observer.updateCount);
}


TEST(WorkflowInstanceTest, EachStateChangeIsLoggedOnce) {
    WorkflowInstance instance;
    instance.handle();   // Pending -> Active
    EXPECT_EQ(1u, instance.getEventLog()->getEntries().size());
}