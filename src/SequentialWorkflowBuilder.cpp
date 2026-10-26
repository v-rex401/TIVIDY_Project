#include "SequentialWorkflowBuilder.h"

#include "WorkflowDefinition.h"
#include "Stage.h"
#include "Task.h"

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

    definition->addStage(stage);
}

void SequentialWorkflowBuilder::buildTask(std::string stageName, std::string title)
{

}

void SequentialWorkflowBuilder::linkHandlers()
{

}

void SequentialWorkflowBuilder::buildTransition(std::string from, std::string to)
{

}