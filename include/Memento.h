#ifndef MEMENTO_H
#define MEMENTO_H

class State;
class WorkItem;
class WorkflowInstance;

class Memento {
friend class WorkflowInstance;

private:
	State* state;
	WorkItem* workItem;

	State* getState();
	void setState(State* s);

public:
	Memento();
	~Memento() = default;
};

#endif
