#include "RuleEngine.h"

bool RuleEngine::evaluate(const AssignmentRule& rule, WorkItem* item, WorkflowInstance* instance) {
    if (item == nullptr) return false;

    if (rule.conditionField == "name") return item->getName() == rule.expectedValue;

    if (rule.conditionField == "title") return item->getTitle() == rule.expectedValue;

    if (rule.conditionField == "requiredPosition") return item->getRequiredPosition() == rule.expectedValue;

    (void)instance;
    
    return false;
}