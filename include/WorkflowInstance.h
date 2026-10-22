#ifndef WORKFLOWINSTANCE_H
#define WORKFLOWINSTANCE_H
#include <string>
#include <vector>

#include "WorkFlowSubject.h"
#include "State.h"
#include "Staff.h"
#include "Memento.h"

class WorkflowDefinition;
class WorkItem;
class CommandHistory;
class EventLog;
class Observer;
class WorkflowVisitor;

class WorkflowInstance : public WorkFlowSubject {

private:
	std::string id;
	State* currentState;
	WorkflowDefinition* definition;
	std::vector<WorkItem*> workItem;
	std::vector<Staff*> participants;
	CommandHistory* history;
	EventLog* eventLog;

public:
	Memento* createMemento();
	void setMemento(Memento* m);

	void attach(Observer* o) override;
	void detach(Observer* o) override;
	void notify();

	void setState(State* s);
	State* getState();
	void handle();

	std::vector<WorkItem*> getWorkItem();
	EventLog* getEventLog();

	void accept(WorkflowVisitor* v);
};

#endif
