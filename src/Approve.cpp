#include "Approve.h"
#include "WorkflowInstance.h"
#include "Cancelled.h"
#include "Completed.h"

Approve::Approve(WorkflowInstance* receiver) : Command(receiver) {}
Approve::~Approve() {}
void Approve::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    // Cannot approve a workflow that has already terminated
    if (dynamic_cast<Cancelled*>(instance->getState()) != nullptr ||
        dynamic_cast<Completed*>(instance->getState()) != nullptr) {
        return;
    }
    saveSnapshot();
    instance->handle();  // delegates the forward transition to the current state
}

void Approve::undo() {
    restoreSnapshot();
}
