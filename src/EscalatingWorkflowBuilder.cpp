#include "EscalatingWorkflowBuilder.h"

#include "WorkflowDefinition.h"
#include "Stage.h"
#include "Task.h"
#include "WorkItem.h"

#include <vector>

EscalatingWorkflowBuilder::~EscalatingWorkflowBuilder()
{
    // Only reached if the caller never took the definition with getWorkflowDefinition()
    // once taken ownership has passed on.
    delete this->definition;
    this->definition = nullptr;
}

void EscalatingWorkflowBuilder::buildDefinition(std::string name)
{
	definition = new WorkflowDefinition();

    definition->setName(name);
}

void EscalatingWorkflowBuilder::buildStage(std::string name)
{
	if(this->definition == nullptr)
    {
        return;
    }

    Stage* stage = new Stage();
    stage->setName(name);

    this->definition->addStage(stage);
}

void EscalatingWorkflowBuilder::buildTask(std::string stageName, std::string title)
{
	if(this->definition == nullptr)
    {
        return;
    }

	for(WorkItem* item : *this->definition->getStages())
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

void EscalatingWorkflowBuilder::linkHandlers()
{
	if(this->definition == nullptr)
    {
        return;
    }

    std::vector<WorkItem*>* stages = this->definition->getStages();

    for(size_t i = stages->size(); i > 1; i--)
    {
        if((*stages)[i - 1] != nullptr && (*stages)[i - 2] != nullptr)
        {
            (*stages)[i - 1]->setNextHandler((*stages)[i - 2]);
        }
    }
}

void EscalatingWorkflowBuilder::buildTransition(std::string from, std::string to)
{
	if(this->definition == nullptr)
    {
        return;
    }

    this->definition->addTransition(from, to);
}

void EscalatingWorkflowBuilder::buildParticipantSlot(std::string stageName, std::string role)
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

void EscalatingWorkflowBuilder::setAssignmentStrategyType(std::string type)
{
	if(this->definition == nullptr)
    {
        return;
    }

    this->definition->setAssignmentStrategyType(type);
}

void EscalatingWorkflowBuilder::setRoutingStrategyType(std::string type)
{
	if(this->definition == nullptr)
    {
        return;
    }

    this->definition->setRoutingStrategyType(type);
}

void EscalatingWorkflowBuilder::setInitialData(std::string key, std::string value)
{
	if(this->definition == nullptr)
    {
        return;
    }

    this->definition->addInitialData(key, value);
}

WorkflowDefinition *EscalatingWorkflowBuilder::getWorkflowDefinition()
{
	 WorkflowDefinition* result = this->definition;

    this->definition = nullptr;

    return result;
}
