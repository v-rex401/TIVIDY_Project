#include "SequentialWorkflowBuilder.h"

#include "WorkflowDefinition.h"
#include "Stage.h"
#include "Task.h"
#include "WorkItem.h"

SequentialWorkflowBuilder::~SequentialWorkflowBuilder()
{
    // Only reached if the caller never took the definition with getWorkflowDefinition()
    // once taken, ownership has passed on
    delete this->definition;
    this->definition = nullptr;
}

void SequentialWorkflowBuilder::buildDefinition(std::string name)
{
    // Starting a new definition discards any part-built one
    delete this->definition;

    definition = new WorkflowDefinition();

    definition->setName(name);
}

void SequentialWorkflowBuilder::buildStage(std::string name)
{
    if(this->definition == nullptr)
    {
        return;
    }

    Stage* stage = new Stage();
    stage->setName(name);

    this->definition->addStage(stage);
}

void SequentialWorkflowBuilder::buildTask(std::string stageName, std::string title)
{
    if(this->definition == nullptr)
    {
        return;
    }

    for(WorkItem* item : *definition->getStages())
    {
        if(item != nullptr && item->getName() == stageName)
        {
            Stage* stage = dynamic_cast<Stage*>(item);

            if(stage != nullptr)
            {
                // The definition format carries a single label per task,
                // so it is used as both the identifier and the display title until
                // the format carries them separately.
                stage->addMember(new Task(title, title));
            }

            return;
        }
    }
}

void SequentialWorkflowBuilder::linkHandlers()
{
    if(this->definition == nullptr)
    {
        return;
    }

    std::vector<WorkItem*>* stages = this->definition->getStages();
    for(size_t i = 0; i + 1 < stages->size(); i++)
    {
        if((*stages)[i] != nullptr)
        {
            (*stages)[i]->setNextHandler((*stages)[i + 1]);
        }
    }
}

void SequentialWorkflowBuilder::buildTransition(std::string from, std::string to)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->addTransition(from, to);
}

void SequentialWorkflowBuilder::buildParticipantSlot(std::string stageName, std::string role)
{
    if(this->definition == nullptr)
    {
        return;
    }

    for(WorkItem* stage : *this->definition->getStages())
    {
        if(stage != nullptr && stage->getName() == stageName)
        {
            this->definition->addParticipantSlot(stageName, role);

            return;
        }
    }
}

void SequentialWorkflowBuilder::setAssignmentStrategyType(std::string type)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->setAssignmentStrategyType(type);
}

void SequentialWorkflowBuilder::setRoutingStrategyType(std::string type)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->setRoutingStrategyType(type);
}

void SequentialWorkflowBuilder::setInitialData(std::string key, std::string value)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->addInitialData(key, value);
}

WorkflowDefinition* SequentialWorkflowBuilder::getWorkflowDefinition()
{
    WorkflowDefinition* result = this->definition;

    this->definition = nullptr;

    return result;
}