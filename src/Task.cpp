#include "Task.h"

#include <string>

std::string Task::getName() {
	return this->name;
}

std::string Task::getTitle() {
	return this->title;
}

void Task::printHierarchy(std::string indent) {
	// TODO - implement Task::printHierarchy
	throw "Not yet implemented";
}

void Task::handleRequest() {
	// TODO - implement Task::handleRequest
	throw "Not yet implemented";
}

void Task::setNextHandler(WorkItem* next) {
	// TODO - implement Task::setNextHandler
	throw "Not yet implemented";
}
