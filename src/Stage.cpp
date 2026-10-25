#include "Stage.h"

#include "Staff.h"
#include "WorkflowVisitor.h"

#include <iostream>
#include <string>

Staff* Task::getResponsible() {
	return responsible;
}

void Task::setResponsible(Staff* staff) {
	responsible = staff;
}

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

void Stage::handleRequest()
{
    //to be implemented with Chain of Responsibility 
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
    Stage* clonedStage = new Stage();

    clonedStage->name = this->name;
    clonedStage->title = this->title;
    clonedStage->responsible = this->responsible;
    clonedStage->requiredPosition = this->requiredPosition;
    
    for(WorkItem* member : members)
    {
        if(member != nullptr)
        {
            clonedStage->addMember(member->clone());
        }
    }

    return clonedStage;
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

std::string Stage::getRequiredPosition() {
    return this->requiredPosition;
}

void Stage::setRequiredPosition(const std::string& position) {
    this->requiredPosition = position;
}

std::string Stage::getSeverity() {
	return this->severity;
}
