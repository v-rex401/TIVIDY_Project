#include "ProgressMonitor.h"
#include "WorkflowEvent.h"
#include "WorkflowInstance.h"
#include <iostream>

ProgressMonitor::ProgressMonitor() {}
ProgressMonitor::~ProgressMonitor() {}

void ProgressMonitor::update(WorkflowEvent* event, WorkflowInstance* context) {
    if (event == nullptr || context == nullptr) {
        return;
    }
    std::cout << "[PROGRESS] [" << event->getTimestamp() << "] " << event->getType() << ": " << event->getDescription() << std::endl;
}

