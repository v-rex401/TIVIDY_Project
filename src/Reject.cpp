#include "Reject.h"
#include "WorkflowInstance.h"
#include "Pending.h"
#include "Cancelled.h"
#include "Completed.h"
Reject::Reject(WorkflowInstance* receiver) : Command(receiver) {}
Reject::~Reject() {}

void Reject::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    // Cannot reject a workflow that has already terminated
    if (dynamic_cast<Cancelled*>(instance->getState()) != nullptr ||
        dynamic_cast<Completed*>(instance->getState()) != nullptr) {
        return;
    }
    saveSnapshot();
    instance->setState(new Pending());  // send back for reassessment
}

void Reject::undo() {
    restoreSnapshot();
}
