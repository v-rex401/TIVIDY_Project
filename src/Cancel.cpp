#include "Cancel.h"
#include "WorkflowInstance.h"
#include "Cancelled.h"

Cancel::Cancel(WorkflowInstance* receiver) : Command(receiver) {}
Cancel::~Cancel() {}

void Cancel::execute() {
    WorkflowInstance* instance = getReceiver();
    if (instance == nullptr) {
        return;
    }
    saveSnapshot();
    instance->setState(new Cancelled());  // the instance takes ownership of the new state
}

void Cancel::undo() {
    restoreSnapshot();
}
