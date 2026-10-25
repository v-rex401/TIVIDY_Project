#ifndef EVENTBASEDROUTING_H
#define EVENTBASEDROUTING_H

#include <vector>
#include "RoutingStrategy.h"
#include "Threshold.h"

class EventBasedRouting : public RoutingStrategy
{
private:
	std::vector<Threshold> thresholds;

public:
	EventBasedRouting(const std::vector<Threshold>& initialThresholds);
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context) override;
};

#endif
