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
    // The instance records a state change in its own log before announcing it,
    // so the event is usually already there. EventLog::addEntry() takes
    // ownership, so appending the same pointer twice would make the log's
    // destructor delete it twice
    const std::vector<WorkflowEvent*>& entries = log->getEntries();
    if (std::find(entries.begin(), entries.end(), event) != entries.end()) {
        return;
    }
    // A new event: ownership passes to the log. The caller must not delete it.
    log->addEntry(event);
}
