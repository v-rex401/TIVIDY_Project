#ifndef PENDING_H
#define PENDING_H
#include "State.h"

class Pending : public State {


public:
	void handle(WorkflowInstance* ctx);
};

#endif
