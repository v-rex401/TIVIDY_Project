#include "EventBasedRouting.h"

EventBasedRouting::EventBasedRouting(const std::vector<Threshold> &initialThresholds)
{
	this->thresholds = initialThresholds;
}

// TODO: item needs to be used
Stage *EventBasedRouting::selectItemState(WorkItem * /* item */, WorkflowInstance *context)
{
	if (item == nullptr || context == nullptr) return nullptr;

	const double metricValue = context->getEventMetric();

	Stage* selectedStage = nullptr;
	double highestTriggeredLimit = 0.0;

	for (const Threshold& th : thresholds) {
		if (th.isTriggered(metricValue)) {
			const double limit = th.getLimit();

			if (selectedStage == nullptr || limit > highestTriggeredLimit) {
				highestTriggeredLimit = limit;
				selectedStage = th.getTargetStage();
			}

		}
	}
	return selectedStage;
}
