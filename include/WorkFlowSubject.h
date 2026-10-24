#ifndef WORKFLOWSUBJECT_H
#define WORKFLOWSUBJECT_H
#include "Observer.h"
#include <vector>

class WorkFlowSubject {
public:
    virtual ~WorkFlowSubject() {}
  
    virtual void attach(Observer* o) = 0;
   
    virtual void detach(Observer* o) = 0;
    
    virtual void notify() = 0;
   
    std::vector<Observer*> getObserverList();
protected:
    std::vector<Observer*> observerList;
};

#endif // WORKFLOWSUBJECT_H
