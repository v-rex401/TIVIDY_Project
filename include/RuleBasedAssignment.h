#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

#include <vector>
#include "AssignmentStrategy.h"
#include "AssignmentRule.h"
#include "RuleEngine.h"

class RuleBasedAssignment : public AssignmentStrategy {

private:
	std::vector<AssignmentRule> rules;
	RuleEngine ruleEngine;
	std::vector<Staff*> roster;

public:
	RuleBasedAssignment(const std::vector<AssignmentRule>& initialRules, const std::vector<Staff*>& pool);
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances);
};

#endif