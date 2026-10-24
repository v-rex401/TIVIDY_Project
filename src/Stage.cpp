#include "Stage.h"

#include "Staff.h"
#include "WorkflowVisitor.h"

#include <iostream>
#include <string>

void Stage::addMember(WorkItem* c)
{
    if(c == nullptr)
    {
        return;
    }

    members.push_back(c);
}

void Stage::removeMember(WorkItem* c)
{
    if(c == nullptr)
    {
        return;
    }

    for(auto it = members.begin(); it != members.end(); ++it)
    {
        if(*it == c)
        {
            members.erase(it);
            break;
        }
    }
}

void Stage::setNextHandler(WorkItem* next)
{
    WorkItem::setNextHandler(next);

}

void Stage::accept(WorkflowVisitor* v)
{
    if(v == nullptr)
    {
        return;
    }

    v->visitStage(this);
}

WorkItem* Stage::clone()
{
    Stage* cloneStage = new Stage();

    cloneStage->name = this->name;
    cloneStage->title = this->title;
    cloneStage->responsible = this->responsible;
    //unable to deep copy members as base class does not have a clone method
    //virtual clone needs to be added to WorkItem for a full deep copy

    return cloneStage;
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

    for(WorkItem* member : members)
    {
        if(member != nullptr)
        {
            member->printHierarchy(indent + "  ");
        }
    }
    
}
