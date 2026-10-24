#include "Retry.h"
#include "WorkflowInstance.h"

Retry::Retry(WorkflowInstance* receiver) : Command(receiver) {}
Retry::~Retry() {}

void Retry::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    saveSnapshot();
    instance->retry();
}

void Retry::undo() {
    restoreSnapshot();
}
