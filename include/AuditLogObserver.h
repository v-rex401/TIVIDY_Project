#ifndef AUDITLOGOBSERVER_H
#define AUDITLOGOBSERVER_H

#include "Observer.h"
/**
 * @brief ConcreteObserver that records workflow events for auditing.
 * Supports the requirement that a workflow must be able to explain what
 * happened,like who/what changed, and when (history retained in the log).
 */
class AuditLogObserver : public Observer {
public:
    AuditLogObserver();
    ~AuditLogObserver();
    /**
     * @brief Record the event in the audit history of the given instance.
     * @param event   The event to record.
     * @param context The instance on which the event occurred.
     */
    void update(WorkflowEvent* event, WorkflowInstance* context) override;
};

#endif // AUDITLOGOBSERVER_H
