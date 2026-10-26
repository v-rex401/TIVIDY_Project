#ifndef APPROVALCHAINROUTING_H
#define APPROVALCHAINROUTING_H

#include "RoutingStrategy.h"

/**
 * @brief Routes work to standard or escalation stages using a transaction-value threshold
 */
class ApprovalChainRouting : public RoutingStrategy
{
private:
	double threshold;
	Stage* standardStage;
	Stage* escalationStage;

public:
	/**
	 * @brief Constructor for ApprovalChainRouting.
	 */
	ApprovalChainRouting(double limitThreshold, Stage* standard, Stage* escalation);
	
	/**
	 * @brief Escalate when the context value exceeds the threshold
	 */
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context) override;
};

#endif
