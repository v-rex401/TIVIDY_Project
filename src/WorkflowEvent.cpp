/**
 * @file WorkflowEvent.cpp
 * @brief Implementation of a single workflow history record.
 * @author Anke de Frey
 * @date 2026-10-22
 */
#include "WorkflowEvent.h"

WorkflowEvent::WorkflowEvent(const string& type, const string& description, const string& timestamp) {
    this->type = type;
    this->description = description;
    this->timestamp = timestamp;
}

string WorkflowEvent::getType() const{
	return this->type;
}

string WorkflowEvent::getDescription() const{
	return this->description;
}

string WorkflowEvent::getTimestamp() const{
	return this->timestamp;
}
