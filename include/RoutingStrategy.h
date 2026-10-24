#ifndef ROUTINGSTRATEGY_H
#define ROUTINGSTRATEGY_H

#include "WorkItem.h"
#include "WorkflowInstance.h"
#include "Stage.h"

class RoutingStrategy {

public:
	virtual Stage* selectItemState(WorkItem* item, WorkflowInstance* context) = 0;
};

#endif
