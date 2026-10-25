#include "RoleBasedAssignment.h"

RoleBasedAssignment::RoleBasedAssignment(const std::vector<Staff *> &pool)
{
    this->roster = pool;
}

// TODO: Context must be used
Staff *RoleBasedAssignment::selectAssignmentItem(WorkItem *item, WorkflowInstance * /* context */)
{
    if (!item)
        return nullptr;

    for (auto *staff : roster)
    {
        if (staff && staff->getTitle() == item->getTitle())
            return staff;
    }
    return nullptr;
}
