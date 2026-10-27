#ifndef APPROVALCHAINROUTING_H
#define APPROVALCHAINROUTING_H

#include <string>

#include "RoutingStrategy.h"

/**
 * @brief Routes work to a standard or escalation stage on a numeric threshold.
 *
 * ConcreteStrategy of the Strategy pattern. The value it compares is named by
 * the scenario and read from the instance's data, so this class knows nothing
 * about what the number means.
 */
class ApprovalChainRouting : public RoutingStrategy
{
private:
	std::string metricKey;
	double threshold;
	Stage* standardStage;
	Stage* escalationStage;

public:
	/**
	 * @brief Construct the strategy.
	 * @param metricKey Name of the instance value to compare, e.g. supplied by the scenario as "transactionValue".
	 * @param limitThreshold Values strictly above this escalate.
	 * @param standard Stage used when the value is at or below the threshold.
	 * @param escalation Stage used when the value exceeds it.
	 */
	ApprovalChainRouting(const std::string& metricKey, double limitThreshold, Stage* standard, Stage* escalation);
	
	/**
	 * @brief Escalate when the context value exceeds the threshold
	 */
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context) override;
};

#endif
