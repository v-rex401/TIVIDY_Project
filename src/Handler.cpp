#include "Handler.h"

#include <iostream>

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
        member->printHierarchy(indent + "  ");
    }
}

void Handler::addMember(WorkItem* Component)
{
	if(Component != nullptr)
    {
        this->members.push_back(Component);
    }
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

	//copy responsible staff pointer
    clonedHandler->responsible = this->responsible;

	//clone each member
    for(WorkItem* member : this->members)
    {
        if(member != nullptr)
        {
            clonedHandler->addMember(member->clone());
        }
    }

    return clonedHandler;
}

Handler::~Handler()
{
	for(WorkItem* member : this->members)
    {
        delete member;
    }
}
