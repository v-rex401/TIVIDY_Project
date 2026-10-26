#include "Reject.h"
#include "WorkflowInstance.h"
#include "Pending.h"

Reject::Reject(WorkflowInstance* receiver) : Command(receiver) {}
Reject::~Reject() {}

void Reject::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    saveSnapshot();
    instance->setState(new Pending());  // send back for reassessment
}

void Reject::undo() {
    restoreSnapshot();
}
