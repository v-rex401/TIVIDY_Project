#ifndef ACTIVE_H
#define ACTIVE_H

#include "WorkflowInstance.h"
#include "State.h"

class Active : public State
{

public:
	void handle(WorkflowInstance *ctx);

	/**
	 * @brief ConcreteState name, used in history and reports.
	 * @return "Active".
	 */
	std::string getName() const;
};

#endif
