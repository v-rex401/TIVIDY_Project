#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

class RuleBasedAssignment : AssignmentStrategy {

public:
	List<AssignmentRule> rules;

	Staff* selectAssignmentItem(WorkItem* item, WorkflowInstance* context);
};

#endif
