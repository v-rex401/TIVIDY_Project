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

public:
	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
