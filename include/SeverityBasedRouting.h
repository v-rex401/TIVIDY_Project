#ifndef SEVERITYBASEDROUTING_H
#define SEVERITYBASEDROUTING_H

#include "RoutingStrategy.h"

/**
 * @brief Routes high-severity work to an emergency stage and other work to a normal stage
 */
class SeverityBasedRouting : public RoutingStrategy
{
private:
	std::string emergencyStageName;
	std::string normalStageName;

public:
	/**
	 * @brief Constructor for severity based routing
	 */
	SeverityBasedRouting(const std::string &normal, const std::string &emergency);

	/**
	 * @brief Select emergency routing for Critical/High severity
	 */
	Stage *selectItemState(WorkItem *item, WorkflowInstance *context) override;
};

#endif
