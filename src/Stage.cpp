#include "Stage.h"

#include "Staff.h"
#include "WorkflowVisitor.h"

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

