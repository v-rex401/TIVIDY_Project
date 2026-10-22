#ifndef MEMENTO_H
#define MEMENTO_H

class State;
class WorkItem;
class WorkflowInstance;

/**
 * @brief Captures and restores the internal state of a WorkflowInstance.
 */
class Memento {
friend class WorkflowInstance;

private:
	State* state;
	WorkItem* workItem;

	State* getState();
	void setState(State* s);

public:
	/**
	 * @brief This constructs a new Memento.
	 */
	Memento();
	~Memento() = default;
};

#endif
