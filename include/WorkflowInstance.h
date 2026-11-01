#ifndef WORKFLOWINSTANCE_H
#define WORKFLOWINSTANCE_H
#include <string>
#include <vector>
#include <memory>
#include <map>

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

class WorkflowInstance : public WorkFlowSubject
{

private:
	std::string id;
	State *currentState;
	WorkflowDefinition *definition;
	std::vector<WorkItem *> workItem;

	// These members aren't following genericness rules
	// double transactionValue;
	// double eventMetric;

	// hence I'm adding Scenario values, keyed by name
	std::map<std::string, double> data;

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
	void notify(WorkflowEvent *e) override;

	void setState(State *s);
	State *getState();
	void handle();

	std::vector<WorkItem *> getWorkItem();
	const std::vector<WorkItem *> &getWorkItems() const;
	void addWorkItem(WorkItem *item);

	/**
	 * @brief Record that a staff member works on this instance.
	 * @param staff The staff member (not owned). Null or already recorded is ignored.
	 */
	void addParticipant(Staff *staff);

	/**
	 * @brief Everyone recorded as working on this instance.
	 * @return The participants, in the order they joined (not owned).
	 */
	const std::vector<Staff *> &getParticipants() const;
	// double getTransactionValue() const;
	// void setTransactionValue(double value);
	// double getEventMetric() const;
	// void setEventMetric(double value);

	/**
	 * @brief Record a named numeric value on this instance.
	 * @param key Name of the value, supplied by the scenario.
	 * @param value The value to store.
	 *
	 * The engine never interprets the key, so an industry can carry whatever
	 * values its rules need without this class knowing about them.
	 */
	void setData(const std::string &key, double value);

	/**
	 * @brief Read a named numeric value.
	 * @param key Name of the value to read.
	 * @param fallback Returned when the key has not been set.
	 * @return The stored value, or @p fallback.
	 */
	double getData(const std::string &key, double fallback = 0.0) const;

	EventLog *getEventLog();

	void accept(WorkflowVisitor *v);

	/**
	 * @brief Gets the unique ID of the workflow instance.
	 * @return the string ID
	 */
	std::string getId() const;

	void setId(const std::string &newId);
	void setDefinition(WorkflowDefinition *def);

	/**
	 * @brief Returns the WorkflowDefintion associated with this particular instance
	 * @return the workflow defintion
	 */
	WorkflowDefinition *getDefinition();
};

#endif
