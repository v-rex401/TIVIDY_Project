#include "AuditLogObserver.h"
#include "EventLog.h"
#include "WorkflowEvent.h"
#include "WorkflowInstance.h"
#include <algorithm>
#include <vector>
 
AuditLogObserver::AuditLogObserver() {}
AuditLogObserver::~AuditLogObserver() {}
 
void AuditLogObserver::update(WorkflowEvent* event, WorkflowInstance* context) {
    if (event == nullptr || context == nullptr) {
        return;
    }
    EventLog* log = context->getEventLog();
    if (log == nullptr) {
        return;
    }
    // WorkflowInstance::notify() announces the latest entry of the log, so the
    // event is normally already recorded. EventLog::addEntry() takes ownership,
    // so appending it a second time would double-free it: skip it.
    std::vector<WorkflowEvent*> entries = log->getEntries();
    if (std::find(entries.begin(), entries.end(), event) != entries.end()) {
        return;
    }
    // A new event: ownership passes to the log. The caller must not delete it.
    log->addEntry(event);
}
