#ifndef APPROVALCHAINROUTING_H
#define APPROVALCHAINROUTING_H

#include "RoutingStrategy.h"
#include "Stage.h"
#include "WorkItem.h"
#include "WorkflowInstance.h"

class ApprovalChainRouting : public RoutingStrategy
{

public:
	double thereshold;

	Stage *selectItemState(WorkItem item, WorkflowInstance *context);
};

#endif
