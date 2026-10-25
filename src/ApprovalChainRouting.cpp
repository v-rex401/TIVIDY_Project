#include "ApprovalChainRouting.h"

ApprovalChainRouting::ApprovalChainRouting(double limitThreshold, Stage* standard, Stage* escalation) {
	this->threshold = limitThreshold;
	this->standardStage = standard;
	this->escalationStage = escalation;
}
	
Stage *ApprovalChainRouting::selectItemState(WorkItem *item, WorkflowInstance *context) {
	double currentTransactionValue = 15000.0;

	if (currentTransactionValue > threshold) {
		return escalationStage;
	}

	return standardStage;
}