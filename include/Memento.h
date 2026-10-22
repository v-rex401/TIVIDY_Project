#ifndef MEMENTO_H
#define MEMENTO_H

class Memento {

private:
	State* state;
	WorkItem* workItem;

	State* getState();

	void setState(State* s);
};

#endif
