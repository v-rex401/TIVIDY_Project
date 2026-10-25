#include "Task.h"

#include <string>
#include <iostream>

std::string Task::getName()
{
	return this->name;
}

std::string Task::getTitle()
{
	return this->title;
}

void Task::printHierarchy(std::string indent)
{
	std::cout << indent << "Task: " << this->name << "\n";
}

void Task::handleRequest()
{
	// TODO: to be implemented with Chain of Responsibility
}

void Task::setNextHandler(WorkItem *next)
{
	WorkItem::setNextHandler(next);
}

void Task::accept(WorkflowVisitor * /* v */)
{
	// TODO: Implement
	return;
}

WorkItem *Task::clone()
{
	// TODO: Implement
	return nullptr;
}