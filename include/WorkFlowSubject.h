#ifndef WORKFLOWSUBJECT_H
#define WORKFLOWSUBJECT_H
#include <vector>
class Observer;
class WorkflowEvent;
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
    virtual void notify(WorkflowEvent* e) = 0;
    /** @brief Return a reference to the observerList. */
    std::vector<Observer *> getObserverList();
protected:
//changed to protected so concrete subject can implement the methods without getObserverList()
    std::vector<Observer*> observerList; 
	
};
 
#endif // WORKFLOWSUBJECT_H
