#ifndef EVENTBASEDROUTING_H
#define EVENTBASEDROUTING_H

#include <vector>
#include <string>
#include "RoutingStrategy.h"
#include "Threshold.h"

/**
 * @brief Routes work according to the first triggered threshold.
 *
 * ConcreteStrategy of the Strategy pattern. The value it compares is named by
 * the scenario and read from the instance's data.
 */
class EventBasedRouting : public RoutingStrategy
{
private:
	std::string metricKey;
	std::vector<Threshold> thresholds;

public:
	/**
	 * @brief Construct the strategy.
	 * @param metricKey Name of the instance value to compare, supplied by the scenario.
	 * @param initialThresholds Thresholds in the order they should be tried.
	 */
	EventBasedRouting(const std::string& metricKey, const std::vector<Threshold>& initialThresholds);
	
	/**
	 * @brief Select the destination associated with the highest triggered limit
	 */
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context) override;
};

#endif
