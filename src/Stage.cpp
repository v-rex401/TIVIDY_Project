#include "Stage.h"
#include "WorkItem.h"

void Stage::addMember(WorkItem* c)
{
    WorkItem* member = c;
    members.push_back(member);
}

