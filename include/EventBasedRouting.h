#ifndef EVENTBASEDROUTING_H
#define EVENTBASEDROUTING_H

class EventBasedRouting : RoutingStrategy {

public:
	List<Threshold> thresholds;

	Stage* selectItemState(WorkItem item, WorkflowInstance* context);
};

#endif
