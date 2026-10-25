#include "SeverityBasedRouting.h"

SeverityBasedRouting::SeverityBasedRouting(Stage *normal, Stage *emergency)
{
    this->normalStage = normal;
    this->emergencyStage = emergency;
}
// TODO: Why is context never used??
Stage *SeverityBasedRouting::selectItemState(WorkItem *item, WorkflowInstance * /* context */)
{
    if (!item)
        return nullptr;

    if (item->getTitle() == "Critical" || item->getTitle() == "HighSeverity")
    {
        return emergencyStage;
    }
    return normalStage;
}