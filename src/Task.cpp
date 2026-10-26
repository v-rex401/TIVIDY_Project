#include "Task.h"
#include "WorkflowVisitor.h"

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
	std::cout << indent << "Task: " << this->title << "\n";
}

void Task::handleRequest()
{
	// TODO: to be implemented with Chain of Responsibility
}

void Task::setNextHandler(WorkItem *next)
{
	WorkItem::setNextHandler(next);
}

void Task::accept(WorkflowVisitor * v)
{
	if(v != nullptr)
    {
        v->visitTask(this);
    }
}

WorkItem *Task::clone()
{
	return new Task(*this);
}

void Task::setTitle(std::string title)
{
	this->title = title;
}