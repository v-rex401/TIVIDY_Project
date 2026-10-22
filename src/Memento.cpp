#include "Memento.h"

State* Memento::getState() {
	return this->state;
}

void Memento::setState(State* s) {
	this->state = s;
}
