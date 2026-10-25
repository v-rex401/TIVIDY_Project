#include "WorkFlowSubject.h"

std::vector<Observer *> &WorkFlowSubject::getObserverList()
{
	return this->observerList;
}
