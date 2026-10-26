#include "WorkflowDirector.h"

#include "WorkflowBuilder.h"

WorkflowDirector::WorkflowDirector(std::string name, std::vector<std::string> stages, std::map<std::string, std::vector<std::string>> tasks)
{
    this->name = name;
    this->stages = stages;
    this->tasks = tasks;
}

void WorkflowDirector::construct(WorkflowBuilder& builder)
{
    builder.buildDefinition(this->name);

    //build all stages
    for(std::string stage : this->stages)
    {
        builder.buildStage(stage);
    }

    //add tasks to their corresponding stages
    for(std::string stage : this->stages)
    {
        auto it = this->tasks.find(stage);

        if(it != this->tasks.end())
        {
            for(std::string title : it->second)
            {
                builder.buildTask(stage, title);
            }
        }
    }

    //create transitions between consecutive stages
    for(size_t i = 0; i + 1 < this->stages.size(); i++)
    {
        builder.buildTransition(this->stages[i], this->stages[i + 1]);
    }

    //link the handlers
    builder.linkHandlers();
}