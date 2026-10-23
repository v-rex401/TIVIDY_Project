#ifndef WORKLOADBASEDASSIGNMENT_H
#define WORKLOADBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class WorkloadBasedAssignment : public AssignmentStrategy {

public:
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
