#include "WorkloadBasedAssignment.h"

WorkloadBasedAssignment::WorkloadBasedAssignment(const std::vector<Staff *> &pool)
{
	this->roster = pool;
}

// TODO: Item is not being used
Staff *WorkloadBasedAssignment::selectAssignmentItem(WorkItem * /* item */, WorkflowInstance *context)
{
	if (!context)
		return nullptr;

	Staff *leastBusyStaff = nullptr;
	int minWorkload = INT_MAX;

	for (auto *staff : roster)
	{
		if (!staff)
			continue;

		int currentCount = 0;
		//
		//
		//
		if (currentCount < minWorkload)
		{
			minWorkload = currentCount;
			leastBusyStaff = staff;
		}
	}

	return leastBusyStaff;
}