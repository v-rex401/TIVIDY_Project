#include "EventBasedRouting.h"

EventBasedRouting::EventBasedRouting(const std::string &metricKey, const std::vector<Threshold> &initialThresholds)
{
	this->metricKey = metricKey;
	this->thresholds = initialThresholds;
}


Stage *EventBasedRouting::selectItemState(WorkItem *item, WorkflowInstance *context)
{
	if (item == nullptr || context == nullptr) return nullptr;

	const double metricValue = context->getData(this->metricKey);

	for (const Threshold& th : thresholds) {
		if (th.isTriggered(metricValue)) {
			return th.getTargetStage();
		}
	}
	return nullptr;
}
