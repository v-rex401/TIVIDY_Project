#ifndef PROGRESSMONITOR_H
#define PROGRESSMONITOR_H

#include "Observer.h"
/**
 * @brief ConcreteObserver that tracks the progress of running workflows.
 * Keeps monitoring information (such as current state) up to
 * date so that running workflows can be inspected without adding reporting
 * code to our core workflow model.
 */
class ProgressMonitor : public Observer {
public:
    ProgressMonitor();
    ~ProgressMonitor();
    /**
     * @brief Refresh progress information after an event.
     * @param event   The event that occurred.
     * @param context The instance whose progress changed.
     */
    void update(WorkflowEvent* event, WorkflowInstance* context) override;
};

#endif // PROGRESSMONITOR_H
