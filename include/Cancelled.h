#ifndef CANCELLED_H
#define CANCELLED_H

#include "State.h"

class Cancelled : public State {

public:
	void handle(WorkflowInstance* ctx);
};

#endif
