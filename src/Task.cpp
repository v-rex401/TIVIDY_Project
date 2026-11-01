/**
 * @file Task.cpp
 * @brief Implementation of the Task class
 * @author Kayla Falconer
 * @date 2026-10-24
 */

#include "Task.h"
#include "WorkflowVisitor.h"
#include "Staff.h"

#include <string>
#include <iostream>

/**
 * @brief Constructs a Task with a name and title
 */
Task::Task(const std::string& name, const std::string& title)
	: name(name), title(title), responsible(nullptr)
{}

/**
 * @brief Gets the name of the Task
 * @return The Task's name
 */
std::string Task::getName()
{
	return this->name;
}

/**
 * @brief Gets the title of the Task
 * @return The Task's title
 */
std::string Task::getTitle()
{
	return this->title;
}

/**
 * @brief Prints the Task in the workflow hierarchy
 * @param indent String used for indentation
 */
void Task::printHierarchy(std::string indent)
{
	std::cout << indent << "Task: " << this->name << "\n";
}

/**
 * @brief Attempts to handle a workflow request based on Staff rank
 * First delegates the request to the responsible Staff member
 * If the Staff member cannot handle the request, it is forwarded to the next WorkItem in the Chain of Responsibility
 * @return true if the request was handled, otherwise false
 */
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

/**
 * @brief Sets the next WorkItem in the Chain of Responsibility
 */
void Task::setNextHandler(WorkItem *next)
{
	WorkItem::setNextHandler(next);
}

/**
 * @brief Accepts a WorkflowVisitor
 * Calls the visitor's visitTask() method to perform an operation on the Task
 */
void Task::accept(WorkflowVisitor *v)
{
	if (v == nullptr)
	{
		return;
	}

	v->visitTask(this);
}

/**
 * @brief Creates a clone of the Task
 * Copies the Task's name, title, severity and required position
 * The responsible Staff pointer is copied without cloning the Staff object.
 * @return Pointer to the cloned Task
 */
WorkItem* Task::clone()
{
    Task* clonedTask = new Task(this->name, this->title);

    clonedTask->severity = this->severity;
    clonedTask->requiredPosition = this->requiredPosition;
    clonedTask->responsible = this->responsible;

    return clonedTask;
}

/**
 * @brief Sets the severity of the Task
 */
void Task::setSeverity(const std::string& severity)
{
    this->severity = severity;
}

/**
 * @brief Gets the Staff member responsible for the Task
 * @return Pointer to the responsible Staff member, or nullptr
 */
Staff* Task::getResponsible()
{
    return responsible;
}

/**
 * @brief Sets the Staff member responsible for the Task
 */
void Task::setResponsible(Staff* staff)
{
    responsible = staff;
}

/**
 * @brief Gets the severity of the Task
 * @return The severity level
 */
std::string Task::getSeverity()
{
    return severity;
}

/**
 * @brief Gets the required Staff position of the Task
 * @return The required Staff position
 */
std::string Task::getRequiredPosition()
{
    return requiredPosition;
}

/**
 * @brief Sets the required Staff position of the Task
 */
void Task::setRequiredPosition(const std::string& position)
{
    requiredPosition = position;
}
