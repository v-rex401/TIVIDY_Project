#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include "Staff.h"
#include "WorkItem.h"
#include "WorkflowInstance.h"


class AssignmentStrategy
{

public:
	virtual Staff *selectAssignmentItem(WorkItem *item, WorkflowInstance *context) = 0;
};

#endif
