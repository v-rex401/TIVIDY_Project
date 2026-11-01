#ifndef PENDING_H
#define PENDING_H
#include "State.h"

class Pending : public State {


public:
	void handle(WorkflowInstance* ctx);
	/**
	 * @brief ConcreteState name, used in history and reports.
	 * @return "Pending".
	 */
	std::string getName() const;
};

#endif
