#include "RuleBasedAssignment.h"

RuleBasedAssignment::RuleBasedAssignment(const std::vector<AssignmentRule>& initialRules, const std::vector<Staff*>& pool) {
    this->rules = initialRules;
    this->roster = pool;
}

Staff* RuleBasedAssignment::selectAssignmentItem(WorkItem* item, WorkflowInstance* context) {
    for (const auto& rule : rules) {
        if (ruleEngine.evaluate(rule, item, context)) {
            for (auto* staff : roster) {
                if (staff && staff->getTitle() == rule.targetTitle) {
                    return staff;
                }
            }
        }
    }
    return nullptr;
}
