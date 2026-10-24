#include "RuleBasedAssignment.h"

RuleBasedAssignment::RuleBasedAssignment(const std::vector<AssignmentRule>& initialRules, const std::vector<Staff*>& pool) {
    this->rules = initialRules;
    this->roster = pool;
}

Staff* RuleBasedAssignment::selectAssignmentItem(WorkItem* item, WorkflowInstance* context) {
    //
}
