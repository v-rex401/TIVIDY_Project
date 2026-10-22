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
	 * @brief Executes the behaviour associated with this specific state.
	 * @param ctx A pointer to the WorkflowInstance (Context) that is changing state.
	 */
	virtual void handle(WorkflowInstance* ctx) = 0;
	
};

#endif
