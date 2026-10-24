#include "Reject.h"
#include "WorkflowInstance.h"

Reject::Reject(WorkflowInstance* receiver) : Command(receiver) {}
Reject::~Reject() {}

void Reject::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    saveSnapshot();
    instance->reject();
}

void Reject::undo() {
    restoreSnapshot();
}
