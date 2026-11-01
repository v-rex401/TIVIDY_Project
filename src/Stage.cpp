/**
 * @file Stage.cpp
 * @brief Implementation of the Stage class
 * @author Kayla Falconer
 * @date 2026-10-24
 */

#include "Stage.h"

#include "Staff.h"
#include "WorkflowVisitor.h"

#include <iostream>
#include <string>
#include <algorithm>

/**
 * @brief Constructs a Stage with a name and title
 */
Stage::Stage(const std::string &name, const std::string &title)
    : name(name), title(title), responsible(nullptr)
{
}

/**
 * @brief Destroys the Stage and its remaining WorkItems
 * Deletes all WorkItems still contained in the Stage and clears the collection
 */
Stage::~Stage()
{
    for (WorkItem *member : members)
    {
        delete member;
    }

    members.clear();
}

/**
 * @brief Adds a WorkItem to the Stage
 * Prevents null pointers and duplicate pointers from being added to the Stage's collection
 */
void Stage::addMember(WorkItem *c)
{
    if (c == nullptr)
    {
        return;
    }

    // prevents same pointer from being inserted more than once
    // this helps prevent a potential double deletion
    if (std::find(this->members.begin(), this->members.end(), c) != this->members.end())
    {
        return;
    }

    this->members.push_back(c);
}

/**
 * @brief Removes a WorkItem from the Stage
 * Removes the WorkItem pointer from the collection without deleting the object
 */
void Stage::removeMember(WorkItem *c)
{
    if (c == nullptr)
    {
        return;
    }

    for (auto it = this->members.begin(); it != this->members.end(); ++it)
    {
        if (*it == c)
        {
            // Removed becase it is a double free causing segfault
            //  delete *it;
            this->members.erase(it);

            return;
        }
    }
}

/**
 * @brief Attempts to handle a request based on the required Staff rank
 * First delegates the request to the responsible Staff member
 * If the Staff member cannot handle it, the request is forwarded to the next WorkItem in the Chain of Responsibility
 * @return true if the request was handled, otherwise false
 */
bool Stage::handleRequest(int rank)
{
    if (this->responsible != nullptr)
    {
        if (this->responsible->handleRequest(rank))
        {
            return true;
        }
    }

    if (getNextHandler() != nullptr)
    {
        return getNextHandler()->handleRequest(rank);
    }

    return false;
}

/**
 * @brief Sets the next WorkItem in the Chain of Responsibility
 */
void Stage::setNextHandler(WorkItem *next)
{
    WorkItem::setNextHandler(next);
}

/**
 * @brief Accepts a WorkflowVisitor
 * Calls the visitor's visitStage() method to perform an operation on the Stage
 */
void Stage::accept(WorkflowVisitor *v)
{
    if (v == nullptr)
    {
        return;
    }

    v->visitStage(this);
}

/**
 * @brief Creates a clone of the Stage
 * Copies the Stage's attributes and creates independent clones of its contained WorkItems
 * The responsible Staff pointer is copied without cloning the Staff object
 * @return Pointer to the cloned Stage
 */
WorkItem *Stage::clone()
{
    Stage *clonedStage = new Stage();

    clonedStage->name = this->name;
    clonedStage->title = this->title;
    clonedStage->severity = this->severity;
    clonedStage->requiredPosition = this->requiredPosition;
    clonedStage->responsible = this->responsible;

    for (WorkItem *member : members)
    {
        if (member != nullptr)
        {
            clonedStage->addMember(member->clone());
        }
    }

    return clonedStage;
}

/**
 * @brief Gets the Staff member responsible for the Stage
 * @return Pointer to the responsible Staff member, or nullptr
 */
Staff *Stage::getResponsible()
{
    return responsible;
}

/**
 * @brief Sets the Staff member responsible for the Stage
 */
void Stage::setResponsible(Staff *staff)
{
    responsible = staff;
}

/**
 * @brief Gets the severity of the Stage
 * @return The severity level
 */
std::string Stage::getSeverity()
{
    return severity;
}

/**
 * @brief Sets the severity of the Stage
 */
void Stage::setSeverity(const std::string &severity)
{
    this->severity = severity;
}

/**
 * @brief Gets the required Staff position of the Stage
 * @return The required Staff position
 */
std::string Stage::getRequiredPosition()
{
    return requiredPosition;
}

/**
 * @brief Sets the required Staff position of the Stage
 * @param position The required Staff position
 */
void Stage::setRequiredPosition(const std::string &position)
{
    requiredPosition = position;
}

/**
 * @brief Gets the WorkItems contained in the Stage
 * @return Constant reference to the collection of WorkItem pointers
 */
const std::vector<WorkItem *> &Stage::getMembers() const
{
    return members;
}

/**
 * @brief Gets the name of the Stage
 * @return The Stage's name
 */
std::string Stage::getName()
{
    return this->name;
}

/**
 * @brief Gets the title of the Stage
 * @return The Stage's title
 */
std::string Stage::getTitle()
{
    return this->title;
}

/**
 * @brief Prints the Stage and its contained WorkItem hierarchy
 * Recursively calls printHierarchy() on each contained WorkItem, increasing the indentation for each level
 */
void Stage::printHierarchy(std::string indent)
{
    std::cout << indent << "Stage: " << this->name << "\n";

    for (WorkItem *member : members)
    {
        if (member != nullptr)
        {
            member->printHierarchy(indent + "  ");
        }
    }
}

/**
 * @brief Sets the name of the Stage
 */
void Stage::setName(std::string name)
{
    this->name = name;
}
