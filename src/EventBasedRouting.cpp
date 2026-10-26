#include "EventBasedRouting.h"

EventBasedRouting::EventBasedRouting(const std::vector<Threshold> &initialThresholds)
{
	this->thresholds = initialThresholds;
}

// TODO: item needs to be used
Stage *EventBasedRouting::selectItemState(WorkItem * /*item*/, WorkflowInstance *context)
{
	if (context == nullptr) return nullptr;

	const double metricValue = context->getEventMetric();

	for (const Threshold& th : thresholds) {
		if (th.isTriggered(metricValue)) {
			return th.getTargetStage();
		}
	}
	return nullptr;
}
