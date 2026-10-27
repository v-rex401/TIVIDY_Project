#ifndef STATE_H
#define STATE_H
#include <string>

class WorkflowInstance;

/**
 * @brief The base State interface for the State design pattern.
 */
class State {

public:
	/**
	 * @brief The virtual destructor for the State interface.
	 */
	virtual ~State() = default;

	/**
	 * @brief The name of this state, used in history and reports.
	 * @return The ConcreteState's name, e.g. "Pending".
	 */
	virtual std::string getName() const = 0;
	
	/**
	 * @brief Executes the behaviour associated with this specific state.
	 * @param ctx A pointer to the WorkflowInstance (Context) that is changing state.
	 */
	virtual void handle(WorkflowInstance* ctx) = 0;
	
};

#endif
