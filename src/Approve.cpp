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
    instance->approve();
}

void Approve::undo() {
    restoreSnapshot();
}
