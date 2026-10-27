#include "Command.h"
#include "Memento.h"
#include "WorkflowInstance.h"

Command::Command(WorkflowInstance* receiver) : receiver(receiver), snapshot(nullptr) {}
Command::~Command() {
    delete snapshot;
}

WorkflowInstance* Command::getReceiver() const {
    return receiver;
}

void Command::saveSnapshot() {
    if (receiver == nullptr) {
        return;
    }
    delete snapshot;
    snapshot = receiver->createMemento();
}

void Command::restoreSnapshot() {
    if (receiver == nullptr || snapshot == nullptr) {
        return;
    }
    receiver->setMemento(snapshot);
}
