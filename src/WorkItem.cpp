#include "WorkItem.h"
#include "WorkflowVisitor.h"

void WorkItem::setNextHandler(WorkItem* next) {
	this->successor = next;
}

void WorkItem::accept(WorkflowVisitor* v)
{
    if(v != nullptr)
    {
        v->visitWorkItem(this);
    }
}