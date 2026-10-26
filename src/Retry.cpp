#include "Retry.h"
#include "WorkflowInstance.h"
#include "Pending.h"
#include "Cancelled.h"
#include "Completed.h"
Retry::Retry(WorkflowInstance* receiver) : Command(receiver) {}
Retry::~Retry() {}

void Retry::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    // Cannot retry a workflow that has already terminated
    if (dynamic_cast<Cancelled*>(instance->getState()) != nullptr ||
        dynamic_cast<Completed*>(instance->getState()) != nullptr) {
        return;
    }
    saveSnapshot();
    instance->setState(new Pending());  // reset to Pending
    instance->handle();                 // then immediately drive the state forward
}

void Retry::undo() {
    restoreSnapshot();
}
