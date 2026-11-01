/**
 * @file WorkflowDefinition.cpp
 * @brief Implementation of the WorkflowDefinition class
 * @author Kayla Falconer
 * @date 2026-10-26
 */

#include "WorkflowDefinition.h"

#include "WorkItem.h"
#include "WorkflowVisitor.h"

/**
 * @brief Accepts a WorkflowVisitor
 * Passes the visitor to each stage in the workflow definition, allowing the visitor to perform operations on the WorkItems
 * @param wv Pointer to the WorkflowVisitor
 */
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

/**
 * @brief Creates a clone of the WorkflowDefinition
 * Copies the workflow's name, transitions, participant roles, strategy types and initial data
 * Creates independent clones of the contained WorkItems and reconstructs the Chain of Responsibility links between top-level stages
 * @return Pointer to the cloned WorkflowDefinition
 */
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

/**
 * @brief Destroys the WorkflowDefinition and its owned stages
 * Deletes all WorkItems stored in the stages collection and clears the collection
 */
WorkflowDefinition::~WorkflowDefinition()
{
    for (WorkItem *stage : stages)
    {
        delete stage;
    }

    stages.clear();
}

/**
 * @brief Adds a WorkItem as a stage in the workflow definition
 * Ignores null pointers. The WorkflowDefinition takes ownership of the added WorkItem
 * @param s Pointer to the WorkItem to add
 */
void WorkflowDefinition::addStage(WorkItem *s)
{
    if (s == nullptr)
    {
        return;
    }

    this->stages.push_back(s);
}

/**
 * @brief Gets the stages of the workflow
 * @return Pointer to the vector containing the workflow's stages
 */
std::vector<WorkItem *> *WorkflowDefinition::getStages()
{
    return &this->stages;
}

/**
 * @brief Sets the name of the workflow
 * @param name The workflow name
 */
void WorkflowDefinition::setName(std::string name)
{
    this->name = name;
}

/**
 * @brief Gets the name of the workflow
 * @return The workflow's name
 */
std::string WorkflowDefinition::getName() const
{
    return this->name;
}

/**
 * @brief Adds a transition between two stages
 * Stores the starting and destination stage names in their respective collections
 * @param from Name of the starting stage
 * @param to Name of the destination stage
 */
void WorkflowDefinition::addTransition(std::string from, std::string to)
{
    transitionFrom.push_back(from);
    transitionTo.push_back(to);
}

/**
 * @brief Adds a participant role to a workflow stage
 * Associates a stage name with a participant role
 * @param stageName Name of the stage
 * @param role Role assigned to the stage
 */
void WorkflowDefinition::addParticipantSlot(std::string stageName, std::string role)
{
    this->participantStageNames.push_back(stageName);
    this->participantRoles.push_back(role);
}

/**
 * @brief Sets the assignment strategy type for the workflow
 * @param type Name of the assignment strategy type
 */
void WorkflowDefinition::setAssignmentStrategyType(std::string type)
{
    this->assignmentStrategyType = type;
}

/**
 * @brief Sets the routing strategy type for the workflow
 * @param type Name of the routing strategy type
 */
void WorkflowDefinition::setRoutingStrategyType(std::string type)
{
    this->routingStrategyType = type;
}

/**
 * @brief Gets the assignment strategy type
 * @return The assignment strategy type, or an empty string if unset
 */
std::string WorkflowDefinition::getAssignmentStrategyType() const
{
    return this->assignmentStrategyType;
}

/**
 * @brief Gets the routing strategy type
 * @return The routing strategy type, or an empty string if unset
 */
std::string WorkflowDefinition::getRoutingStrategyType() const
{
    return this->routingStrategyType;
}

/**
 * @brief Gets the transitions between workflow stages
 * Combines the starting and destination stage names into pairs, preserving the order in which transitions were added
 * @return Vector of pairs containing starting and destination stage names
 */
std::vector<std::pair<std::string, std::string>> WorkflowDefinition::getTransitions() const
{
    std::vector<std::pair<std::string, std::string>> result;
    for (std::size_t i = 0; i < transitionFrom.size() && i < transitionTo.size(); ++i)
    {
        result.push_back(std::make_pair(transitionFrom[i], transitionTo[i]));
    }
    return result;
}

/**
 * @brief Adds initial data to the workflow
 * Stores the key and its corresponding value in the initial data collections
 * @param key Key identifying the data
 * @param value Value associated with the key
 */
void WorkflowDefinition::addInitialData(std::string key, std::string value)
{
    this->initialDataKeys.push_back(key);
    this->initialDataValues.push_back(value);
}

/**
 * @brief Finds a Stage in the workflow by its name
 * Searches the top-level WorkItems for a matching name
 * @param stageName Name of the Stage to search for
 * @return Pointer to the matching Stage, or nullptr if not found
 */
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