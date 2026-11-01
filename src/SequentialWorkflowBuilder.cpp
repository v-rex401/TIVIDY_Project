/**
 * @file SequentialWorkflowBuilder.cpp
 * @brief Implementation of the SequentialWorkflowBuilder class
 * @author Kayla Falconer
 * @date 2026-10-26
 */

#include "SequentialWorkflowBuilder.h"

#include "WorkflowDefinition.h"
#include "Stage.h"
#include "Task.h"
#include "WorkItem.h"

/**
 * @brief Destroys the SequentialWorkflowBuilder
 * Deletes the constructed WorkflowDefinition if ownership has not been transferred to the caller
 */
SequentialWorkflowBuilder::~SequentialWorkflowBuilder()
{
    // Only reached if the caller never took the definition with getWorkflowDefinition()
    // once taken, ownership has passed on
    delete this->definition;
    this->definition = nullptr;
}

/**
 * @brief Builds a new WorkflowDefinition
 * Deletes any previously constructed definition and creates a new one with the specified name
 * @param name Name of the workflow definition
 */
void SequentialWorkflowBuilder::buildDefinition(std::string name)
{
    // Starting a new definition discards any part-built one
    delete this->definition;

    definition = new WorkflowDefinition();

    definition->setName(name);
}

/**
 * @brief Builds a stage in the sequential workflow
 * Creates a new Stage and adds it to the current WorkflowDefinition
 * @param name Name of the stage to create
 */
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

/**
 * @brief Builds a task within a specified workflow stage
 * Searches for a Stage with the given name and adds a new Task
 * The task title is used as both its identifier and display title
 * @param stageName Name of the stage containing the task
 * @param title Title of the task to create
 */
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

/**
 * @brief Links the workflow stages sequentially
 * Establishes the Chain of Responsibility by setting each stage's next handler to the following stage in the workflow
 */
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

/**
 * @brief Adds a transition between two workflow stages
 * Stores the transition in the current WorkflowDefinition
 * @param from Name of the starting stage
 * @param to Name of the destination stage
 */
void SequentialWorkflowBuilder::buildTransition(std::string from, std::string to)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->addTransition(from, to);
}

/**
 * @brief Adds a participant slot to a workflow stage
 * Searches for the specified stage and associates a participant role with it if the stage exists
 * @param stageName Name of the stage
 * @param role Participant role associated with the stage
 */
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

/**
 * @brief Sets the assignment strategy type for the workflow
 * @param type Name of the assignment strategy type
 */
void SequentialWorkflowBuilder::setAssignmentStrategyType(std::string type)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->setAssignmentStrategyType(type);
}

/**
 * @brief Sets the routing strategy type for the workflow
 * @param type Name of the routing strategy type
 */
void SequentialWorkflowBuilder::setRoutingStrategyType(std::string type)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->setRoutingStrategyType(type);
}

/**
 * @brief Stores initial data for the workflow
 * Adds a key-value pair to the current WorkflowDefinition
 * @param key Key identifying the initial data
 * @param value Value associated with the key
 */
void SequentialWorkflowBuilder::setInitialData(std::string key, std::string value)
{
    if(this->definition == nullptr)
    {
        return;
    }

    this->definition->addInitialData(key, value);
}

/**
 * @brief Retrieves the constructed WorkflowDefinition
 * Transfers ownership of the constructed definition to the caller and resets the builder's definition pointer to nullptr
 * The caller becomes responsible for deleting the returned object
 * @return Pointer to the constructed WorkflowDefinition, or nullptr if no definition is available
 */
WorkflowDefinition* SequentialWorkflowBuilder::getWorkflowDefinition()
{
    WorkflowDefinition* result = this->definition;

    this->definition = nullptr;

    return result;
}