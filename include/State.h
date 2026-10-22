#ifndef STATE_H
#define STATE_H

class State {


public:
	virtual void handle(WorkflowInstance* ctx) = 0;
};

#endif
