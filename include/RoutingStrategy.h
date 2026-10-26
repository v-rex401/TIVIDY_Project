#ifndef ROUTINGSTRATEGY_H
#define ROUTINGSTRATEGY_H

#include "WorkItem.h"
#include "WorkflowInstance.h"
#include "Stage.h"

/**
 * @brief Strategy interface for choosing the next stage for a work item
 */
class RoutingStrategy
{

public:
	/**
	 * @brief Select a destination stage for a work item
	 * @param item Work item being routed
	 * @param context Workflow instance proving routing context
	 * @return Selected stage, or nullptr when routing cannot be determined
	 */
	virtual Stage *selectItemState(WorkItem *item, WorkflowInstance *context) = 0;
	
	/**
	 * @brief Virtual destructor for deletion
	 */
	virtual ~RoutingStrategy() {}
};

#endif
