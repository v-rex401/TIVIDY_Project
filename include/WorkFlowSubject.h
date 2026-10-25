#ifndef WORKFLOWSUBJECT_H
#define WORKFLOWSUBJECT_H
#include "Observer.h"
#include <vector>
/**
 * @brief Abstract Subject (Observer pattern).
 * Declares how observers are registered and notified. WorkflowInstance is the
 * ConcreteSubject and implements all three operations. Observers are only
 * referenced, never owned.
 */
class WorkFlowSubject {
public:
    virtual ~WorkFlowSubject() {}
    /** @brief Register an observer. */
    virtual void attach(Observer* o) = 0;
    /** @brief Unregister an observer. */
    virtual void detach(Observer* o) = 0;
    /** @brief Tell every attached observer that something happened. */
    virtual void notify() = 0;
    /** @brief Return a reference to the observerList. */
    std::vector<Observer *> &getObserverList();
private:
    std::vector<Observer*> observerList; 
	
};
 
#endif // WORKFLOWSUBJECT_H
