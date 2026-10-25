#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class RoleBasedAssignment : public AssignmentStrategy {
private:
	std::vector<Staff*> roster;

public:
	RoleBasedAssignment(const std::vector<Staff*>& pool);
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances);
};

#endif
