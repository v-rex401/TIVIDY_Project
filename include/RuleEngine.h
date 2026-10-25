#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "AssignmentRule.h"
#include "WorkflowInstance.h"
#include "WorkItem.h"

class RuleEngine
{

public:
    // TODO: This should be implemented or this header should be removed
    bool evaluate(const AssignmentRule &rule, WorkItem *item, WorkflowInstance *instance) { return false; };
};

#endif
