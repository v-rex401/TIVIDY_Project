#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "AssignmentRule.h"
#include "WorkflowInstance.h"
#include "WorkItem.h"

class RuleEngine {

public:
    bool evaluate(const AssignmentRule& rule, WorkItem* item, WorkflowInstance* instance);
};

#endif
