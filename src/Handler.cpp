#include "Handler.h"

#include <iostream>
#include <algorithm>

Handler::Handler(std::string name, std::string title)
{
    this->name = name;
    this->title = title;
    this->responsible = nullptr;
}

std::string Handler::getName()
{
	return this->name;
}

std::string Handler::getTitle()
{
	return this->title;
}

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

void Handler::setResponsible(Staff* staff)
{
	this->responsible = staff;
}

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

std::string Handler::getSeverity()
{
    return this->severity;
}

std::string Handler::getRequiredPosition()
{
    return this->requiredPosition;
}

void Handler::setRequiredPosition(const std::string& position)
{
    this->requiredPosition = position;
}

Staff* Handler::getResponsible()
{
    return this->responsible;
}

Handler::~Handler()
{
	for(WorkItem* member : this->members)
    {
        delete member;
    }
}
