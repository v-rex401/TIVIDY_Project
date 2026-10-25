#include "WorkloadBasedAssignment.h"

WorkloadBasedAssignment::WorkloadBasedAssignment(const std::vector<Staff*>& pool) {
	this->roster = pool;
}

Staff* WorkloadBasedAssignment::selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances) {
	if (!context || !item || roster.empty()) return nullptr;

	Staff* leastBusyStaff = nullptr;
	int minWorkload = INT_MAX;

	for (Staff* staff : roster) {
		if (!staff) continue;

		int currentCount = 0;
		for (WorkflowInstance* instance : runningInstances) {
			if (!instance) continue;

			if (instance->getState() == nullptr) continue;

			for (WorkItem* workItem : instance->getWorkItem()) {
				if (!workItem) continue;

				if (workItem->getResponsible() == staff) {
					++currentCount;
				}
			}
		}

		if (currentCount < minWorkload) {
			minWorkload = currentCount;
			leastBusyStaff = staff;
		}
	}

	return leastBusyStaff;
}