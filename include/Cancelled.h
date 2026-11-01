#ifndef CANCELLED_H
#define CANCELLED_H

#include "State.h"

class Cancelled : public State {

public:
	void handle(WorkflowInstance* ctx);

	/**
	 * @brief ConcreteState name, used in history and reports.
	 * @return "Cancelled".
	 */
	std::string getName() const;
};

#endif
