#ifndef WORKFLOWSUBJECT_H
#define WORKFLOWSUBJECT_H

#include <vector>
#include "Observer.h"

class WorkFlowSubject
{

private:
	std::vector<Observer *> observerList;

public:
	virtual void attach(Observer *o) = 0;

	virtual void detach(Observer *o) = 0;

	virtual void notify() = 0;

	std::vector<Observer *> getObserverList();
};

#endif
