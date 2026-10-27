/**
 * @file CommandTests.cpp
 * @brief Behaviour tests for the Command pattern participants.
 */
#include <gtest/gtest.h>
#include "Active.h"
#include "Approve.h"
#include "Cancel.h"
#include "Cancelled.h"
#include "CommandHistory.h"
#include "Invoker.h"
#include "Memento.h"
#include "Pending.h"
#include "Reject.h"
#include "Retry.h"
#include "State.h"
#include "WorkflowInstance.h"
/** Test state: counts how often the instance hands it work. */
class SpyState : public State {
public:
    explicit SpyState(int* handled) : handled(handled) {}
    void handle(WorkflowInstance*) override { ++*handled; }
private:
    int* handled;
};

TEST(CancelTest, ExecuteMovesTheInstanceToCancelled) {
    WorkflowInstance instance;
    Cancel cancel(&instance);
    cancel.execute();
    EXPECT_TRUE(dynamic_cast<Cancelled*>(instance.getState()) != NULL);
}

TEST(ApproveTest, ExecuteHandsTheWorkToTheCurrentState) {
    WorkflowInstance instance;
    int handled = 0;
    instance.setState(new SpyState(&handled));
    Approve approve(&instance);
    approve.execute();
    EXPECT_EQ(1, handled);
}

TEST(RejectTest, ExecuteSendsTheWorkflowBackToPending) {
    WorkflowInstance instance;
    instance.setState(new Active());
    Reject reject(&instance);
    reject.execute();
    EXPECT_TRUE(dynamic_cast<Pending*>(instance.getState()) != NULL);
}

TEST(RetryTest, ExecuteRestartsTheWorkflowWithAFreshState) {
    WorkflowInstance instance;
    State* before = new Active();
    instance.setState(before);
    Retry retry(&instance);
    retry.execute();
    EXPECT_NE(before, instance.getState());
}

TEST(CommandTest, UndoRestoresTheStateBeforeTheCommand) {
    WorkflowInstance instance;
    State* before = new Active();
    instance.setState(before);
    Cancel cancel(&instance);
    Reject reject(&instance);
    Retry retry(&instance);
    cancel.execute();
    cancel.undo();
    EXPECT_EQ(before, instance.getState());
    reject.execute();
    reject.undo();
    EXPECT_EQ(before, instance.getState());
    retry.execute();
    retry.undo();
    EXPECT_EQ(before, instance.getState());
}

TEST(CommandTest, ApproveRejectAndRetryDoNothingOnceCancelled) {
    WorkflowInstance instance;
    State* cancelled = new Cancelled();
    instance.setState(cancelled);
    Approve(&instance).execute();
    Reject(&instance).execute();
    Retry(&instance).execute();
    EXPECT_EQ(cancelled, instance.getState());
}

TEST(CommandTest, NullReceiverIsSafe) {
    Cancel cancel(NULL);
    Approve approve(NULL);
    EXPECT_NO_THROW(cancel.execute());
    EXPECT_NO_THROW(approve.execute());
    EXPECT_NO_THROW(cancel.undo());
}

TEST(InvokerTest, ExecuteCommandRunsTheCommandItWasGiven) {
    WorkflowInstance instance;
    Cancel cancel(&instance);
    Invoker invoker;
    invoker.setCommand(&cancel);
    invoker.executeCommand();
    EXPECT_TRUE(dynamic_cast<Cancelled*>(instance.getState()) != NULL);
}

TEST(InvokerTest, SaveThenUndoRollsTheInstanceBack) {
    WorkflowInstance instance;
    State* before = new Active();
    instance.setState(before);
    CommandHistory history;
    Invoker invoker(&history);
    Cancel cancel(&instance);
    invoker.setCommand(&cancel);
    invoker.save(instance.createMemento());
    invoker.executeCommand();
    invoker.undo(NULL);
    EXPECT_EQ(before, instance.getState());
}

TEST(InvokerTest, NoCommandOrHistoryIsSafe) {
    WorkflowInstance instance;
    Invoker invoker;
    EXPECT_NO_THROW(invoker.executeCommand());
    EXPECT_NO_THROW(invoker.undo(NULL));
    EXPECT_NO_THROW(invoker.save(instance.createMemento()));
}

TEST(CommandHistoryTest, RestoreReturnsTheLatestMementoOrNull) {
    WorkflowInstance instance;
    CommandHistory history;
    EXPECT_TRUE(history.restore() == NULL);
    history.save(instance.createMemento());
    Memento* latest = instance.createMemento();
    history.save(latest);
    EXPECT_EQ(latest, history.restore());
}
