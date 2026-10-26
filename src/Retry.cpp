#include "Retry.h"
#include "WorkflowInstance.h"
#include "Pending.h"
Retry::Retry(WorkflowInstance* receiver) : Command(receiver) {}
Retry::~Retry() {}

void Retry::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    saveSnapshot();
    instance->setState(new Pending());  // reset to Pending
    instance->handle();                 // then immediately drive the state forward
}

void Retry::undo() {
    restoreSnapshot();
}
