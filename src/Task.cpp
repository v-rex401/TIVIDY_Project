#include "Task.h"

#include <string>
#include <iostream>

std::string Task::getName() {
	return this->name;
}

std::string Task::getTitle() {
	return this->title;
}

void Task::printHierarchy(std::string indent) {
	std::cout << indent << "Task: " << this->name << "\n";

}

void Task::handleRequest() {
	//to be implemented with Chain of Responsibility 
}

void Task::setNextHandler(WorkItem* next) {
	// TODO - implement Task::setNextHandler
	throw "Not yet implemented";
}
