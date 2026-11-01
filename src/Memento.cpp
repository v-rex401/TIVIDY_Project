/**
 * @file Memento.cpp
 * @brief Implementation of the Memento state.
 * @author Jezelle Govender
 * @date 2026-10-24
 */

#include "Memento.h"
#include "State.h"
#include "WorkItem.h"

Memento::Memento() : state(nullptr), workItem(nullptr) {

}

State* Memento::getState() { //accessible by friend WorkflowInstance
	return this->state;
}

void Memento::setState(State* s) {
	this->state = s;
}
