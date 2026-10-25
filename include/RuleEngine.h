#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "AssignmentRule.h"
#include "WorkflowInstance.h"
#include "WorkItem.h"

class RuleEngine
{

public:
    // TODO: This is not in the UML this header should be removed
    bool evaluate(const AssignmentRule &rule, WorkItem *item, WorkflowInstance *instance);
};

#endif
