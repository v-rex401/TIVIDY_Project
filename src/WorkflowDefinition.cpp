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

    clonedDefinition->transitionFrom = this->transitionFrom;
    clonedDefinition->transitionTo = this->transitionTo;

    clonedDefinition->participantStageNames = this->participantStageNames;
    clonedDefinition->participantRoles = this->participantRoles;

    clonedDefinition->assignmentStrategyType = this->assignmentStrategyType;
    clonedDefinition->routingStrategyType = this->routingStrategyType;

    clonedDefinition->initialDataKeys = this->initialDataKeys;
    clonedDefinition->initialDataValues = this->initialDataValues;

    for(WorkItem* stage : this->stages)
    {
        if(stage != nullptr)
        {
            clonedDefinition->addStage(stage->clone());
        }
    }

    return clonedDefinition;
}

WorkflowDefinition::~WorkflowDefinition()
{
    for(WorkItem* stage : stages)
    {
        delete stage;
    }

    stages.clear();
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

void WorkflowDefinition::setName(std::string name)
{
    this->name = name;
}

void WorkflowDefinition::addTransition(std::string from, std::string to)
{
    transitionFrom.push_back(from);
    transitionTo.push_back(to);
}

void WorkflowDefinition::addParticipantSlot(std::string stageName, std::string role)
{
    this->participantStageNames.push_back(stageName);
    this->participantRoles.push_back(role);
}

void WorkflowDefinition::setAssignmentStrategyType(std::string type)
{
    this->assignmentStrategyType = type;
}

void WorkflowDefinition::setRoutingStrategyType(std::string type)
{
    this->routingStrategyType = type;
}

void WorkflowDefinition::addInitialData(std::string key, std::string value)
{
    this->initialDataKeys.push_back(key);
    this->initialDataValues.push_back(value);
}
