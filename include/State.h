#ifndef STATE_H
#define STATE_H
#include <string>

class WorkflowInstance;

class State {

public:
	virtual ~State() = default;
	virtual void handle(WorkflowInstance* ctx) = 0;
	
};

#endif
