#ifndef WORKLOADBASEDASSIGNMENT_H
#define WORKLOADBASEDASSIGNMENT_H

#include <climits>
#include "AssignmentStrategy.h"

/**
 * @brief Assigns work to a roster member with the smallest observed workload
 */
class WorkloadBasedAssignment : public AssignmentStrategy {
private:
	std::vector<Staff*> roster;

public:
	/**
	 * @brief Constructor with the staff roster
	 */
	WorkloadBasedAssignment(const std::vector<Staff*>& pool);
	
	/**
	 * @brief Select a roster member with the lowest count of assigned work
	 */
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances);
};

#endif
