#include "WorkFlowSubject.h"

vector<Observer*> WorkFlowSubject::getObserverList() {
	return this->observerList;
}
