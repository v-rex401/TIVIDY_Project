#ifndef ACTIVE_H
#define ACTIVE_H

#include "WorkflowInstance.h"
#include "State.h"

class Active : public State
{

public:
	void handle(WorkflowInstance *ctx);
};

#endif
