#include "SeverityBasedRouting.h"
#include "WorkflowDefinition.h"

SeverityBasedRouting::SeverityBasedRouting(const std::string &normal, const std::string &emergency)
{
    this->normalStageName = normal;
    this->emergencyStageName = emergency;
}

Stage *SeverityBasedRouting::selectItemState(WorkItem *item, WorkflowInstance *context)
{
    if (item == nullptr || context == nullptr)
        return nullptr;

    WorkflowDefinition *def = context->getDefinition();
    if (def == nullptr)
        return nullptr;

    const std::string severity = item->getSeverity();
    if (severity == "Critical" || severity == "High")
    {
        return def->findStage(emergencyStageName);
    }
    return def->findStage(normalStageName);
}