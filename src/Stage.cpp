#include "Stage.h"

#include "Staff.h"
#include "WorkflowVisitor.h"

#include <iostream>
#include <string>
#include <algorithm>

Stage::Stage(const std::string &name, const std::string &title)
    : name(name), title(title), responsible(nullptr)
{
}
Stage::~Stage()
{
    for (WorkItem *member : members)
    {
        delete member;
    }

    members.clear();
}

void Stage::addMember(WorkItem* c)
{
    if(c == nullptr)
    {
        return;
    }

    //prevents same pointer from being inserted more than once
    //this helps prevent a potential double deletion
    if(std::find(this->members.begin(), this->members.end(), c) != this->members.end())
    {
        return;
    }

    this->members.push_back(c);
}

void Stage::removeMember(WorkItem* c)
{
    if(c == nullptr)
    {
        return;
    }

    for(auto it = this->members.begin(); it != this->members.end(); ++it)
    {
        if(*it == c)
        {
            delete *it;
            this->members.erase(it);

            return;
        }
    }
}

bool Stage::handleRequest(int rank)
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

void Stage::setNextHandler(WorkItem *next)
{
    WorkItem::setNextHandler(next);
}

void Stage::accept(WorkflowVisitor *v)
{
    if (v == nullptr)
    {
        return;
    }

    v->visitStage(this);
}

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

Staff *Stage::getResponsible()
{
    return responsible;
}

void Stage::setResponsible(Staff *staff)
{
    responsible = staff;
}

std::string Stage::getSeverity()
{
    return severity;
}

std::string Stage::getRequiredPosition()
{
    return requiredPosition;
}

void Stage::setRequiredPosition(const std::string &position)
{
    requiredPosition = position;
}

const std::vector<WorkItem *> &Stage::getMembers() const
{
    return members;
}

std::string Stage::getName()
{
    return this->name;
}

std::string Stage::getTitle()
{
    return this->title;
}

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

void Stage::setName(std::string name)
{
    this->name = name;
}
