#include "WorkloadBasedAssignment.h"

WorkloadBasedAssignment::WorkloadBasedAssignment(const std::vector<Staff *> &pool)
{
	this->roster = pool;
}

Staff* WorkloadBasedAssignment::selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances) {
	if (context == nullptr || item == nullptr) return nullptr;

	Staff *leastBusyStaff = nullptr;
	int minWorkload = INT_MAX;

	for (Staff* staff : roster) {
		if (staff == nullptr) continue;

		int currentCount = 0;
		for (WorkflowInstance* instance : runningInstances) {
			if (instance == nullptr) continue;

			const std::vector<WorkItem*> items = instance->getWorkItem();

			for (WorkItem* assignedItem : items) {
				if (assignedItem != nullptr && assignedItem->getResponsible() == staff) ++currentCount;
			}
		}

		if (currentCount < minWorkload) {
			minWorkload = currentCount;
			leastBusyStaff = staff;
		}
	}

	return leastBusyStaff;
}