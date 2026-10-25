#include "SeverityBasedRouting.h"

SeverityBasedRouting::SeverityBasedRouting(Stage *normal, Stage *emergency)
{
    this->normalStage = normal;
    this->emergencyStage = emergency;
}

Stage *SeverityBasedRouting::selectItemState(WorkItem *item, WorkflowInstance *context) {
    if (item == nullptr || context == nullptr) return nullptr;

    const std::string severity = item->getSeverity();

    if (severity == "Critical" || severity == "High") {
        return emergencyStage;
    }
    return normalStage;
}