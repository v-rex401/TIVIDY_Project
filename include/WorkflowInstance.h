#ifndef WORKFLOWINSTANCE_H
#define WORKFLOWINSTANCE_H

class WorkflowInstance : WorkFlowSubject {

private:
	string id;
	State* currentState;
	WorkflowDefinition* definition;
	vector<WorkItem*> workItem;
	vector<Staff*> participants;
	CommandHistory* history;
	EventLog* eventLog;

public:
	Memento* createMemento();

	void setMemento(Memento* m);

	void attach(Observer* o);

	void detach(Observer* o);

	void setState(State* s);

	State* getState();

	void getWorkItem();

	EventLog* getEventLog();

	void notify();

	void handle();

	void accept(WorkflowVisitor* v);
};

#endif
