#include "Invoker.h"
#include "Command.h"
#include "CommandHistory.h"
#include "Memento.h"
#include "WorkflowInstance.h"

Invoker::Invoker(CommandHistory* history) : command(nullptr), history(history) {}
Invoker::~Invoker() {}

void Invoker::setCommand(Command* c) {
    command = c;
}

void Invoker::executeCommand() {
    if (command == nullptr) {
        return;
    }
    command->execute();
}

void Invoker::save(Memento* m) {
    if (m == nullptr) {
        return;
    }
    if (history == nullptr) {
        delete m;   // nothing can keep it
        return;
    }
    history->save(m);
}

void Invoker::undo(Memento* m) {
    if (command == nullptr) {
        return;
    }
    WorkflowInstance* instance = command->getReceiver();
    if (instance == nullptr) {
        return;
    }
    if (m == nullptr && history != nullptr) {
        m = history->restore();
    }
    if (m == nullptr) {
        return;
    }
    instance->setMemento(m);
}
