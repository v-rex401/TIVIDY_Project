#ifndef WORKLOADBASEDASSIGNMENT_H
#define WORKLOADBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class WorkloadBasedAssignment : public AssignmentStrategy {
private:
	std::vector<Staff*> roster;

public:
	WorkloadBasedAssignment(const std::vector<Staff*>& pool);
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
