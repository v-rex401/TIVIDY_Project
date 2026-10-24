#include "ProgressMonitor.h"
#include "WorkflowEvent.h"
#include "WorkflowInstance.h"
#include <ostream>

ProgressMonitor::ProgressMonitor() {}
ProgressMonitor::~ProgressMonitor() {}

void ProgressMonitor::update(WorkflowEvent* event, WorkflowInstance* context) {
    if (event == nullptr || context == nullptr) {
        return;
    }

    Progress& p = progress[context];
    p.count++;
    p.lastType = event->getType();
    p.lastDescription = event->getDescription();
    p.lastTimestamp = event->getTimestamp();
}
