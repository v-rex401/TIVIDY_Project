#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

#include <vector>
#include "AssignmentStrategy.h"

class RuleBasedAssignment : public AssignmentStrategy {

public:
	std::vector<AssignmentRule> rules;

	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
