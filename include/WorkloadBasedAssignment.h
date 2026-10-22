#ifndef WORKLOADBASEDASSIGNMENT_H
#define WORKLOADBASEDASSIGNMENT_H

class WorkloadBasedAssignment : AssignmentStrategy {


public:
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
