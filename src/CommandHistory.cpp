#include "CommandHistory.h"
#include "Memento.h"

CommandHistory::CommandHistory() {}

CommandHistory::~CommandHistory() {
    for (std::size_t i = 0; i < history.size(); ++i) {
        delete history[i];
    }
}

void CommandHistory::save(Memento* m) {
    if (m == nullptr) {
        return;
    }
    history.push_back(m);
}

Memento* CommandHistory::restore() {
    if (history.empty()) {
        return nullptr;
    }
    return history.back();
}
