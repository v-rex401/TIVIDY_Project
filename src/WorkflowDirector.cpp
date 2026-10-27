#include "WorkflowDirector.h"

#include "WorkflowBuilder.h"

WorkflowDirector::WorkflowDirector(WorkflowBuilder* b)
{
    this->builder = b;
}

void WorkflowDirector::construct(std::string name, std::vector<std::string> stages, std::map<std::string, std::vector<std::string>> tasks)
{
    if(builder == nullptr)
    {
        return;
    }

    builder->buildDefinition(name);

    //build all stages
    for(std::string stage : stages)
    {
        builder->buildStage(stage);
    }

    //add tasks to their corresponding stages
    for(std::string stage : stages)
    {
        auto it = tasks.find(stage);

        if(it != tasks.end())
        {
            for(std::string title : it->second)
            {
                builder->buildTask(stage, title);
            }
        }
    }

    //create transitions between consecutive stages
    for(size_t i = 0; i + 1 < stages.size(); i++)
    {
        builder->buildTransition(stages[i], stages[i + 1]);
    }

    //link the handlers
    builder->linkHandlers();
}