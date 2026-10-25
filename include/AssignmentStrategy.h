#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include "Staff.h"
#include "WorkItem.h"
#include "WorkflowInstance.h"

class AssignmentStrategy
{

public:
	virtual Staff *selectAssignmentItem(WorkItem *item, WorkflowInstance *context, const std::vector<WorkflowInstance*>& runningInstances) = 0;
	virtual ~AssignmentStrategy() {}
};

#endif
