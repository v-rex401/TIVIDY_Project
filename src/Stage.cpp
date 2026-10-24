#include "Stage.h"

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

