/**
 * @file WorkflowDirector.cpp
 * @brief Implementation of the WorkflowDirector class
 * @author Kayla Falconer
 * @date 2026-10-26
 */

#include "WorkflowDirector.h"

#include "WorkflowBuilder.h"

/**
 * @brief Constructs a WorkflowDirector with the specified builder
 * Stores a pointer to the WorkflowBuilder used to construct workflow definitions
 * The director does not take ownership of the builder
 * @param b Pointer to the WorkflowBuilder
 */
WorkflowDirector::WorkflowDirector(WorkflowBuilder* b)
{
    this->builder = b;
}

/**
 * @brief Constructs a workflow using the specified stages and tasks
 * Directs the WorkflowBuilder to create a workflow definition, add stages, assign tasks to their corresponding stages, create transitions between consecutive stages and link the workflow handlers
 * Construction is skipped if the builder is nullptr
 * @param name Name of the workflow to construct
 * @param stages Vector containing the names of the workflow stages
 * @param tasks Map associating each stage name with its task titles
 */
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
