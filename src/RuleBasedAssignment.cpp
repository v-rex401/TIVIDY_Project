#include "RuleBasedAssignment.h"

RuleBasedAssignment::RuleBasedAssignment(const std::vector<AssignmentRule>& initialRules, const std::vector<Staff*>& pool) {
    this->rules = initialRules;
    this->roster = pool;
}

Staff* RuleBasedAssignment::selectAssignmentItem(WorkItem* item, WorkflowInstance* context, const std::vector<WorkflowInstance*>& runningInstances) {
    if (item == nullptr) return nullptr;

    for (const auto& rule : rules) {
        if (!ruleEngine.evaluate(rule, item, context)) {
            continue;
        }
        
        for (Staff* staff : roster) {
            if (staff != nullptr && staff->getTitle() == rule.targetTitle) {
                return staff;
            }
        }
    }
    return nullptr;
}
