#ifndef EVENTBASEDROUTING_H
#define EVENTBASEDROUTING_H

#include <vector>
#include "RoutingStrategy.h"
#include "Threshold.h"

/**
 * @brief Routes work according to the highest triggered event-metric threshold
 */
class EventBasedRouting : public RoutingStrategy
{
private:
	std::vector<Threshold> thresholds;

public:
	/**
	 * @brief Constructor for event thresholds
	 */
	EventBasedRouting(const std::vector<Threshold>& initialThresholds);
	
	/**
	 * @brief Select the destination associated with the highest triggered limit
	 */
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context) override;
};

#endif
