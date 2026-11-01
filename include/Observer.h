#ifndef OBSERVER_H
#define OBSERVER_H

class WorkflowEvent;
class WorkflowInstance;
/**
 * @brief Observer (Observer pattern).
 *
 * Interested parties (audit logging, progress monitoring etc..) implement this
 * interface and attach themselves to a WorkFlowSubject (the WorkflowInstance).
 * They are notified whenever a workflow event occurs, without the instance
 * knowing what they do with it.
 * gof roles:
 *  - Subject          : WorkFlowSubject (WorkflowInstance is ConcreteSubject)
 *  - Observer         : Observer
 *  - ConcreteObserver : AuditLogObserver, ProgressMonitor
 */
class Observer {
public:
    virtual ~Observer();
    /**
     * @brief Called by the subject when something happens.
     * @param event   What happened (type, description, timestamp).
     * @param context The WorkflowInstance on which it happened.
     */
    virtual void update(WorkflowEvent* event, WorkflowInstance* context) = 0;
};

#endif // OBSERVER_H
