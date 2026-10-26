#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

/**
 * @brief Assigns work to the first roster whose title matches the required position
 */
class RoleBasedAssignment : public AssignmentStrategy {
private:
	std::vector<Staff*> roster;

public:
	/**
	 * @brief Constructor with staff roster
	 */
	RoleBasedAssignment(const std::vector<Staff*>& pool);
	
	/**
	 * @brief Return the first staff member whose title matches the required position
	 */
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances);
};

#endif
