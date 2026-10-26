#include "WorkflowDirector.h"

#include "WorkflowBuilder.h"

WorkflowDirector::WorkflowDirector(WorkflowBuilder * b)
{
	this->builder = b;
}

void WorkflowDirector::construct(std::string name, std::vector<std::string> stages, std::map<std::string, std::vector<std::string>> tasks)
{
	if(this->builder == nullptr)
    {
        return;
    }

    //create workflow definition
    this->builder->buildDefinition(name);

    //add all stages
    for(std::string stageName : stages)
    {
        this->builder->buildStage(stageName);
    }

	//add tasks to corresponding stages
    for(std::string stageName : stages)
    {
        auto it = tasks.find(stageName);

        if(it != tasks.end())
        {
            for(std::string taskTitle : it->second)
            {
                this->builder->buildTask(stageName, taskTitle);
            }
        }
    }

	//create transitions between consecutive stages
    for(size_t i = 0; i + 1 < stages.size(); i++)
    {
        this->builder->buildTransition(stages[i], stages[i + 1]);
    }

    //link Chain of Responsibility handlers
    this->builder->linkHandlers();
}
