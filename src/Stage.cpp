#include "Stage.h"

#include "Staff.h"
#include "WorkflowVisitor.h"

#include <iostream>
#include <string>

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

void Stage::addMember(WorkItem *c)
{
    if (c == nullptr)
    {
        return;
    }

    members.push_back(c);
}

void Stage::removeMember(WorkItem *c)
{
    if (c == nullptr)
    {
        return;
    }

    for (auto it = members.begin(); it != members.end(); ++it)
    {
        if (*it == c)
        {
            members.erase(it);
            break;
        }
    }
}

void Stage::handleRequest()
{
    // to be implemented with Chain of Responsibility
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
