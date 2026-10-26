#include "Approve.h"
#include "WorkflowInstance.h"
Approve::Approve(WorkflowInstance* receiver) : Command(receiver) {}
Approve::~Approve() {}

void Approve::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    saveSnapshot();
    instance->handle();  // delegates the forward transition to the current state
}

void Approve::undo() {
    restoreSnapshot();
}
