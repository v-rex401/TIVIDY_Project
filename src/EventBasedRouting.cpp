#include "EventBasedRouting.h"

EventBasedRouting::EventBasedRouting(const std::vector<Threshold>& initialThresholds) {
	this->thresholds = initialThresholds;
}

Stage *EventBasedRouting::selectItemState(WorkItem *item, WorkflowInstance *context)
{
	if (!context) return nullptr;

	double runtimeMetricValue = 85.0;

	Stage* highestTriggeredStage = nullptr;
	double maximumLimitMet = -1.0;

	for (const auto& th : thresholds) {
		if (th.isTriggered(runtimeMetricValue)) {
			highestTriggeredStage = th.getTargetStage();
		}
	}
	return highestTriggeredStage;
}
