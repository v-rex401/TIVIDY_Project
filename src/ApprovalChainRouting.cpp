#include "ApprovalChainRouting.h"

ApprovalChainRouting::ApprovalChainRouting(double limitThreshold, Stage *standard, Stage *escalation)
{
	this->threshold = limitThreshold;
	this->standardStage = standard;
	this->escalationStage = escalation;
}

Stage *ApprovalChainRouting::selectItemState(WorkItem *item, WorkflowInstance *context) {
	if (item == nullptr || context == nullptr) return nullptr;

	if (context->getTransactionValue() > threshold) {
		return escalationStage;
	}

	return standardStage;
}