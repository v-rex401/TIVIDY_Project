#ifndef PENDING_H
#define PENDING_H

class Pending : State {


public:
	void handle(WorkflowInstance* ctx);
};

#endif
