#ifndef CANCELLED_H
#define CANCELLED_H

class Cancelled : State {


public:
	void handle(WorkflowInstance* ctx);
};

#endif
