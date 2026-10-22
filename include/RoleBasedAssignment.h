#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H

class RoleBasedAssignment : AssignmentStrategy {


public:
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
