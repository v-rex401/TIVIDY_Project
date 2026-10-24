/**
 * @file EventLog.cpp
 * @brief Implementation of the workflow execution history.
 * @author Anke de Frey
 * @date 2026-10-24
 */

#include "EventLog.h"

EventLog::EventLog(){}

EventLog::~EventLog()
{
	for(int i = 0; i < (int)this->entries.size(); i++){
		delete this->entries[i];
	}
	this->entries.clear();
}

void EventLog::addEntry(WorkflowEvent *e)
{
	if(e== NULL){
		return;
	}
	this->entries.push_back(e);
}

const std::vector<WorkflowEvent *> &EventLog::getEntries() const
{
	return this->entries;
}
