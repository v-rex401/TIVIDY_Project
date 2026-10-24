#ifndef APPROVALCHAINROUTING_H
#define APPROVALCHAINROUTING_H

#include "RoutingStrategy.h"

class ApprovalChainRouting : public RoutingStrategy
{
private:
	double threshold;

public:
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context);
};

#endif
