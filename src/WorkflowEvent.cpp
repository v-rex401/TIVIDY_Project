#include "WorkflowEvent.h"

string WorkflowEvent::getType() {
	return this->type;
}

string WorkflowEvent::getDescription() {
	return this->description;
}

string WorkflowEvent::getTimestamp() {
	return this->timestamp;
}
