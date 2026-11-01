#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "AssignmentRule.h"
#include "WorkflowInstance.h"
#include "WorkItem.h"

/**
 * @brief Evaluates assignment rules against work items and workflow context
 */
class RuleEngine
{

public:
    /**
     * @brief Determine whether an assignment rule matches a work item
     * @param rule Rule containing the field and expected value
     * @param item Work item to inspect
     * @param instance Workflow context reserved for context-dependent rules
     * @return True when the field equals the expected value
     */
    bool evaluate(const AssignmentRule &rule, WorkItem *item, WorkflowInstance *instance);
};

#endif
