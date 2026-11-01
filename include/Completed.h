#ifndef COMPLETED_H
#define COMPLETED_H
#include "State.h"

class Completed : public State {


public:
	void handle(WorkflowInstance* ctx);

	/**
	 * @brief ConcreteState name, used in history and reports.
	 * @return "Completed".
	 */
	std::string getName() const;
};

#endif
