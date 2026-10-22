#ifndef WORKFLOWSUBJECT_H
#define WORKFLOWSUBJECT_H

class WorkFlowSubject {

private:
	vector<Observer*> observerList;

public:
	virtual void attach(Observer* o) = 0;

	virtual void detach(Observer* o) = 0;

	virtual void notify() = 0;

	vector<Observer*> getObserverList();
};

#endif
