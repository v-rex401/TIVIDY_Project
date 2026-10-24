#include "SeverityBasedRouting.h"

SeverityBasedRouting::SeverityBasedRouting(Stage* normal, Stage* emergency) {
    this->normalStage = normal;
    this->emergencyStage = emergency;
}

Stage *SeverityBasedRouting::selectItemState(WorkItem *item, WorkflowInstance *context) {
    //
}