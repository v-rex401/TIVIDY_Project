#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

#include <vector>
#include "AssignmentStrategy.h"
#include "AssignmentRule.h"
#include "RuleEngine.h"

/**
 * @brief Assigns work using ordered rules and a staff roster
 */
class RuleBasedAssignment : public AssignmentStrategy {

private:
	std::vector<AssignmentRule> rules;
	RuleEngine ruleEngine;
	std::vector<Staff*> roster;

public:
	/**
	 * @brief Constructor with ordered assignment rules and roster
	 */
	RuleBasedAssignment(const std::vector<AssignmentRule>& initialRules, const std::vector<Staff*>& pool);
	
	/**
	 * @brief Evaluate rules in order and select a roster member matching the rule target
	 */
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances);
};

#endif