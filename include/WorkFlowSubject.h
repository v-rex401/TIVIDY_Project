#ifndef WORKFLOWSUBJECT_H
#define WORKFLOWSUBJECT_H

#include <vector>
class Observer;
class WorkFlowSubject {
public:
    virtual ~WorkFlowSubject() {}
  
    virtual void attach(Observer* o);
   
    virtual void detach(Observer* o);
    
    virtual void notify() = 0;
   
    std::vector<Observer*> getObserverList();
protected:
    std::vector<Observer*> observerList;
};

#endif // WORKFLOWSUBJECT_H
