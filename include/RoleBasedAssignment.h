#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class RoleBasedAssignment : public AssignmentStrategy {

public:
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
