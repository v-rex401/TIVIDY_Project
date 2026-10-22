#ifndef ROUTINGSTRATEGY_H
#define ROUTINGSTRATEGY_H

class RoutingStrategy {


public:
	virtual Stage* selectItemState(WorkItem item, WorkflowInstance* context) = 0;
};

#endif
