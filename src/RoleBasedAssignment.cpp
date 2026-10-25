#include "RoleBasedAssignment.h"

RoleBasedAssignment::RoleBasedAssignment(const std::vector<Staff*>& pool) {
    this->roster = pool;
}

Staff* RoleBasedAssignment::selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances) {
    if (item == nullptr) return nullptr;

    (void)context;

    for (Staff* staff : roster) {
        if (staff != nullptr && staff->getTitle() == item->getRequiredPosition()) {
            return staff;
        }
    }
    return nullptr;
}
