#include "WorkflowDefinition.h"

#include "WorkItem.h"
#include "WorkflowVisitor.h"

void WorkflowDefinition::accept(WorkflowVisitor *wv)
{
    if (wv == nullptr)
    {
        return;
    }

    for (WorkItem *stage : this->stages)
    {
        if (stage != nullptr)
        {
            stage->accept(wv);
        }
    }
}

WorkflowDefinition *WorkflowDefinition::clone()
{
    WorkflowDefinition *clonedDefinition = new WorkflowDefinition();

    clonedDefinition->name = this->name;

    clonedDefinition->transitionFrom = this->transitionFrom;
    clonedDefinition->transitionTo = this->transitionTo;

    clonedDefinition->participantStageNames = this->participantStageNames;
    clonedDefinition->participantRoles = this->participantRoles;

    clonedDefinition->assignmentStrategyType = this->assignmentStrategyType;
    clonedDefinition->routingStrategyType = this->routingStrategyType;

    clonedDefinition->initialDataKeys = this->initialDataKeys;
    clonedDefinition->initialDataValues = this->initialDataValues;

    for (WorkItem *stage : this->stages)
    {
        if (stage != nullptr)
        {
            clonedDefinition->addStage(stage->clone());
        }
    }

    // Stage::clone() copies one stage, so the links the builder made between
    // stages (the Chain of Responsibility) are rebuilt here by position.
    std::vector<WorkItem *> *copies = clonedDefinition->getStages();
    std::vector<WorkItem *> originals;
    for (WorkItem *stage : this->stages)
    {
        if (stage != nullptr)
        {
            originals.push_back(stage);
        }
    }
    for (std::size_t i = 0; i < originals.size(); ++i)
    {
        WorkItem *next = originals[i]->getNextHandler();
        for (std::size_t j = 0; j < originals.size() && next != nullptr; ++j)
        {
            if (originals[j] == next)
            {
                (*copies)[i]->setNextHandler((*copies)[j]);
                break;
            }
        }
    }

    return clonedDefinition;
}

WorkflowDefinition::~WorkflowDefinition()
{
    for (WorkItem *stage : stages)
    {
        delete stage;
    }

    stages.clear();
}

void WorkflowDefinition::addStage(WorkItem *s)
{
    if (s == nullptr)
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

std::string WorkflowDefinition::getName() const
{
    return this->name;
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

std::string WorkflowDefinition::getAssignmentStrategyType() const
{
    return this->assignmentStrategyType;
}

std::string WorkflowDefinition::getRoutingStrategyType() const
{
    return this->routingStrategyType;
}

std::vector<std::pair<std::string, std::string>> WorkflowDefinition::getTransitions() const
{
    std::vector<std::pair<std::string, std::string>> result;
    for (std::size_t i = 0; i < transitionFrom.size() && i < transitionTo.size(); ++i)
    {
        result.push_back(std::make_pair(transitionFrom[i], transitionTo[i]));
    }
    return result;
}

void WorkflowDefinition::addInitialData(std::string key, std::string value)
{
    this->initialDataKeys.push_back(key);
    this->initialDataValues.push_back(value);
}

Stage *WorkflowDefinition::findStage(const std::string &stageName)
{

    for (WorkItem *item : stages)
    {
        if (item != nullptr && item->getName() == stageName)
        {
            return static_cast<Stage *>(item);
        }
    }
    return nullptr;
}