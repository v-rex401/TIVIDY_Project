#ifndef SEVERITYBASEDROUTING_H
#define SEVERITYBASEDROUTING_H

#include "RoutingStrategy.h"

class SeverityBasedRouting : public RoutingStrategy {
public:
	Stage* selectItemState(WorkItem item, WorkflowInstance* context) override;
};

#endif
