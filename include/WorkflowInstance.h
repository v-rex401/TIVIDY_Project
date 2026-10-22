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

/**
 * @brief The main context class that manages a workflow.
 * It interacts with the State, Memento, and Observer pattern.
 */

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

	/**
	 * @brief Constructs a new WorkflowInstance.
	 */
	WorkflowInstance();

	/**
	 * @brief Creates a Memento that contains a snapshot of the current state.
	 * @return A pointer to the newly created Memento.
	 */
	Memento* createMemento();

	/**
	 * @brief Restores the state of the workflow from a Memento.
	 * @package m Pointer to the Memento to restore from.
	 */
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
