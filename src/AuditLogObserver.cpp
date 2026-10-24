#include "AuditLogObserver.h"
#include "EventLog.h"
#include "WorkflowEvent.h"
#include "WorkflowInstance.h"

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
    // The EventLog owns its entries due to composition, so ownership of
    // the event passes to the log here. caller doesnt delete it, and the
    // same event must not be passed to this observer twice.
    log->addEntry(event);
}
