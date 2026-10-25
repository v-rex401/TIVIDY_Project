#include "RuleEngine.h"

bool RuleEngine::evaluate(const AssignmentRule& rule, WorkItem* item, WorkflowInstance* instance) {
    if (item && item->getTitle() == rule.expectedValue) {
        return true;
    }
    return false;
}