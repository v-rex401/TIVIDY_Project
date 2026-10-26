#include "SequentialWorkflowBuilder.h"

#include "WorkflowDefinition.h"
#include "Stage.h"
#include "Task.h"
#include "WorkItem.h"

void SequentialWorkflowBuilder::buildDefinition(std::string name)
{
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
                Task* task = new Task();
                task->setTitle(title);

                stage->addMember(task);
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

}

void SequentialWorkflowBuilder::setAssignmentStrategyType(std::string type)
{

}

void SequentialWorkflowBuilder::setRoutingStrategyType(std::string type)
{

}

void SequentialWorkflowBuilder::setInitialData(std::string key, std::string value)
{

}

WorkflowDefinition* SequentialWorkflowBuilder::getWorkflowDefinition()
{

}