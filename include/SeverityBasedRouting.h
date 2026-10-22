#ifndef SEVERITYBASEDROUTING_H
#define SEVERITYBASEDROUTING_H

class SeverityBasedRouting : RoutingStrategy {


public:
	Stage selectItemStage(WorkItem item, WorkflowInstance* context);
};

#endif
