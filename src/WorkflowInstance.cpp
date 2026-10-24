#include "WorkflowEvent.h"
#include <algorithm>
#include "Observer.h"
#include "EventLog.h"

Memento* WorkflowInstance::createMemento() {
	// TODO - implement WorkflowInstance::createMemento
	throw "Not yet implemented";
}

void WorkflowInstance::setMemento(Memento* m) {
	// TODO - implement WorkflowInstance::setMemento
	throw "Not yet implemented";
}

void WorkflowInstance::attach(Observer* o) {
	if (o == nullptr) {
        return;
    }
    if (std::find(observerList.begin(), observerList.end(), o) != observerList.end()) {
        return;  // already attached
    }
    observerList.push_back(o);
}

void WorkflowInstance::detach(Observer* o) {
	observerList.erase(std::remove(observerList.begin(), observerList.end(), o), observerList.end());
}

void WorkflowInstance::setState(State* s) {
	// TODO - implement WorkflowInstance::setState
	throw "Not yet implemented";
}

State* WorkflowInstance::getState() {
	// TODO - implement WorkflowInstance::getState
	throw "Not yet implemented";
}

void WorkflowInstance::getWorkItem() {
	// TODO - implement WorkflowInstance::getWorkItem
	throw "Not yet implemented";
}

EventLog* WorkflowInstance::getEventLog() {
	// TODO - implement WorkflowInstance::getEventLog
	throw "Not yet implemented";
}

void WorkflowInstance::notify() {
	// most recent entry of log is the announced event.
    EventLog* log = getEventLog();
    if (log == nullptr) {
        return;
    }
    std::vector<WorkflowEvent*> entries = log->getEntries();
    if (entries.empty()) {
        return;
    }
    WorkflowEvent* latest = entries.back();
    // use copy to iterate, may detach during update
    std::vector<Observer*> snapshot = observerList;
    for (std::size_t i = 0; i < snapshot.size(); ++i) {
        snapshot[i]->update(latest, this);
    }
}

void WorkflowInstance::handle() {
	// TODO - implement WorkflowInstance::handle
	throw "Not yet implemented";
}

void WorkflowInstance::accept(WorkflowVisitor* v) {
	// TODO - implement WorkflowInstance::accept
	throw "Not yet implemented";
}
