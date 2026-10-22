#ifndef COMPLETED_H
#define COMPLETED_H
#include "State.h"

class Completed : public State {


public:
	void handle(WorkflowInstance* ctx);
};

#endif
