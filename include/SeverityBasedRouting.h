#ifndef SEVERITYBASEDROUTING_H
#define SEVERITYBASEDROUTING_H

#include "RoutingStrategy.h"

class SeverityBasedRouting : public RoutingStrategy {
private:
	Stage* normalStage;
	Stage* emergencyStage;

public:
	SeverityBasedRouting(Stage* normal, Stage* emergency);
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context) override;
};

#endif
