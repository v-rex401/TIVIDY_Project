#ifndef WORKFLOWINSTANCE_H
#define WORKFLOWINSTANCE_H
#include <string>
#include <vector>
#include <memory>

#include "WorkFlowSubject.h"
#include "State.h"
#include "Staff.h"
#include "Memento.h"

class WorkflowDefinition;
class WorkItem;
class Stage;
class CommandHistory;
class EventLog;
class Observer;
class WorkflowVisitor;

/**
 * @brief The main context class that manages a workflow.
 * It interacts with the State, Memento, and Observer pattern.
 */

class WorkflowInstance : public WorkFlowSubject
{

private:
	std::string id;
	State *currentState;
	WorkflowDefinition *definition;
	std::vector<WorkItem *> workItem;
	double transactionValue;
	double eventMetric;
	std::vector<Staff *> participants;
	CommandHistory *history;
	EventLog *eventLog;

	std::vector<std::unique_ptr<State>> ownedStates; // Added to keep all state objects alive for the lifetime of the instance.

public:
	/**
	 * @brief Constructs a new WorkflowInstance.
	 */
	WorkflowInstance();
	virtual ~WorkflowInstance();

	/**
	 * @brief Creates a Memento that contains a snapshot of the current state.
	 * @return A pointer to the newly created Memento.
	 */
	Memento *createMemento();

	/**
	 * @brief Restores the state of the workflow from a Memento.
	 * @param m Pointer to the Memento to restore from.
	 */
	void setMemento(Memento *m);

	void attach(Observer *o) override;
	void detach(Observer *o) override;
	void notify() override;

	void setState(State *s);
	State *getState();
	void handle();

	std::vector<WorkItem *> getWorkItem() const;
	void addWorkItem(WorkItem *item);
	Stage *getCurrentStage() const;
	void setCurrentStage(Stage *stage);
	double getTransactionValue() const;
	void setTransactionValue(double value);
	double getEventMetric() const;
	void setEventMetric(double value);
	EventLog *getEventLog();

	void accept(WorkflowVisitor *v);

	/**
	 * @brief Gets the unique ID of the workflow instance.
	 * @return the string ID
	 */
	std::string getId() const;

	void setId(const std::string &newId);
	void setDefinition(WorkflowDefinition *def);
};

#endif
