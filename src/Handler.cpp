/**
 * @file Handler.cpp
 * @brief Implementation of the Handler class
 * @author Kayla Falconer
 * @date 2026-10-27
 */

#include "Handler.h"

#include <iostream>
#include <algorithm>

/**
 * @brief Constructs a Handler with a name and title
 * @param name Short identifier for the Handler
 * @param title Human-readable title of the Handler
 */
Handler::Handler(std::string name, std::string title)
{
    this->name = name;
    this->title = title;
    this->responsible = nullptr;
}

/**
 * @brief Gets the name of the Handler
 * @return The Handler's name
 */
std::string Handler::getName()
{
	return this->name;
}

/**
 * @brief Gets the title of the Handler
 * @return The Handler's title
 */
std::string Handler::getTitle()
{
	return this->title;
}

/**
 * @brief Prints the Handler and its contained WorkItems
 * Recursively prints the hierarchy of contained WorkItems, increasing the indentation for each level
 * @param indent String used for indentation
 */
void Handler::printHierarchy(std::string indent)
{
	std::cout << indent << this->name << "\n";

    for(WorkItem* member : this->members)
    {
        if(member != nullptr)
        {
            member->printHierarchy(indent + "  ");
        }
    }
}

/**
 * @brief Adds a WorkItem to the Handler
 * Prevents null pointers and duplicate pointers from being added to the Handler's collection
 * @param Component Pointer to the WorkItem to add
 */
void Handler::addMember(WorkItem* Component)
{
    if(Component == nullptr)
    {
        return;
    }

    //prevents same pointer from being inserted more than once
    //this helps prevent a potential double deletion
    if(std::find(this->members.begin(), this->members.end(), Component) != this->members.end())
    {
        return;
    }

    this->members.push_back(Component);
}

/**
 * @brief Removes and deletes a WorkItem from the Handler
 * Searches for the specified WorkItem, deletes the object and removes its pointer from the collection
 * @param Component Pointer to the WorkItem to remove and delete
 */
void Handler::removeMember(WorkItem* Component)
{
	for(auto it = this->members.begin(); it != this->members.end(); ++it)
    {
        if(*it == Component)
        {
            delete *it;
            this->members.erase(it);

            return;
        }
    }
}

/**
 * @brief Assigns a responsible Staff member to the Handler
 * Stores a pointer to the Staff member without taking ownership
 * @param staff Pointer to the responsible Staff member
 */
void Handler::setResponsible(Staff* staff)
{
	this->responsible = staff;
}

/**
 * @brief Attempts to handle a workflow request based on Staff rank
 * First delegates the request to the responsible Staff member
 * If the Staff member cannot handle the request, it is forwarded to the next WorkItem in the Chain of Responsibility
 * @return true if the request was handled, otherwise false
 */
bool Handler::handleRequest(int rank)
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
 * @brief Creates a clone of the Handler
 * Copies the Handler's attributes and creates independent clones of its contained WorkItems
 * The responsible Staff pointer is copied without cloning the Staff object
 * The successor pointer is not copied
 * @return Pointer to the cloned Handler
 */
WorkItem* Handler::clone()
{
    Handler* clonedHandler = new Handler(this->name, this->title);

    clonedHandler->severity = this->severity;
    clonedHandler->requiredPosition = this->requiredPosition;
    clonedHandler->responsible = this->responsible;

    for(WorkItem* member : this->members)
    {
        if(member != nullptr)
        {
            clonedHandler->addMember(member->clone());
        }
    }

    return clonedHandler;
}

/**
 * @brief Gets the severity of the Handler
 * @return The severity level
 */
std::string Handler::getSeverity()
{
    return this->severity;
}

/**
 * @brief Sets the severity of the Handler
 */
void Handler::setSeverity(const std::string& severity)
{
    this->severity = severity;
}

/**
 * @brief Gets the required Staff position of the Handler
 * @return The required Staff position
 */
std::string Handler::getRequiredPosition()
{
    return this->requiredPosition;
}

/**
 * @brief Sets the required Staff position of the Handler
 * @param position The required Staff position
 */
void Handler::setRequiredPosition(const std::string& position)
{
    this->requiredPosition = position;
}

/**
 * @brief Gets the Staff member responsible for the Handler
 * @return Pointer to the responsible Staff member, or nullptr
 */
Staff* Handler::getResponsible()
{
    return this->responsible;
}

/**
 * @brief Destroys the Handler and its contained WorkItems
 * Deletes all WorkItems remaining in the Handler's collection
 * Does not delete the responsible Staff member
 */
Handler::~Handler()
{
	for(WorkItem* member : this->members)
    {
        delete member;
    }
}
