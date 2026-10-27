#include "Task.h"
#include "WorkflowVisitor.h"
#include "Staff.h"

#include <string>
#include <iostream>

Task::Task(const std::string& name, const std::string& title)
	: name(name), title(title), responsible(nullptr)
{}

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

bool Task::handleRequest(int rank)
{
    if(this->responsible != nullptr)
    {
        if(this->responsible->handleRequest(rank))
        {
            return true;
        }
    }

    if(getNextHandler() != nullptr)
    {
        return getNextHandler()->handleRequest(rank);
    }

    return false;
}

void Task::setNextHandler(WorkItem *next)
{
	WorkItem::setNextHandler(next);
}

void Task::accept(WorkflowVisitor *v)
{
	if (v == nullptr)
	{
		return;
	}

	v->visitTask(this);
}

WorkItem* Task::clone()
{
    Task* clonedTask = new Task(this->name, this->title);

    clonedTask->severity = this->severity;
    clonedTask->requiredPosition = this->requiredPosition;
    clonedTask->responsible = this->responsible;

    return clonedTask;
}

Staff* Task::getResponsible()
{
    return responsible;
}

void Task::setResponsible(Staff* staff)
{
    responsible = staff;
}

std::string Task::getSeverity()
{
    return severity;
}

std::string Task::getRequiredPosition()
{
    return requiredPosition;
}

void Task::setRequiredPosition(const std::string& position)
{
    requiredPosition = position;
}
