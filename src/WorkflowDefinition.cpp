#include "WorkflowDefinition.h"

#include "WorkItem.h"
#include "WorkflowVisitor.h"

void WorkflowDefinition::accept(WorkflowVisitor* wv)
{
	if(wv == nullptr)
    {
        return;
    }

    for(WorkItem* stage : this->stages)
    {
        if(stage != nullptr)
        {
            stage->accept(wv);
        }
    }
}

WorkflowDefinition *WorkflowDefinition::clone()
{
	WorkflowDefinition* clonedDefinition = new WorkflowDefinition();

    clonedDefinition->name = this->name;

    for(WorkItem* stage : this->stages)
    {
        if(stage != nullptr)
        {
            clonedDefinition->addStage(stage->clone());
        }
    }

    return clonedDefinition;
}

void WorkflowDefinition::addStage(WorkItem* s)
{
	if(s == nullptr)
    {
        return;
    }

    this->stages.push_back(s);
}

std::vector<WorkItem *> *WorkflowDefinition::getStages()
{
	return &this->stages;
}

void WorkflowDefinition::setBuilder(WorkflowBuilder* b)
{
    this->builder = b;
}

void WorkflowDefinition::setName(std::string name)
{
    this->name = name;
}
