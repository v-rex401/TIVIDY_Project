#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include "Staff.h"
#include "WorkItem.h"
#include "WorkflowInstance.h"

/**
 * @brief Strategy interface for selecting staff to perform a work item
 * 
 * Concrete strategies implement role-, rule- or workload-based selection.
 * The running-instance collection is supplied for strategies that need to
 * inspect existing work assignments.
 */
class AssignmentStrategy
{

public:
	/**
	 * @brief Select a staff member for a work item.
	 * @param item Work item that needs an assignee.
	 * @param context Workflow instance containing the work item.
	 * @param runningInstances Currently running workflows used for context.
	 * @return Selected staff or nullptr when no member exists.
 	*/
	virtual Staff *selectAssignmentItem(WorkItem *item, WorkflowInstance *context, const std::vector<WorkflowInstance*>& runningInstances) = 0;
	
	/**
	 * @brief Virtual destructor for deletion
	 */
	virtual ~AssignmentStrategy() {}
};

#endif
