#ifndef WORKFLOWDEFINITION_H
#define WORKFLOWDEFINITION_H

#include <string>
#include <vector>
#include <utility>
#include "Stage.h"

class WorkItem;
class WorkflowVisitor;

/**
 * @brief Defines the structure and stages of a workflow
 */

class WorkflowDefinition
{

private:
	std::string name;
	std::vector<WorkItem *> stages;
	std::vector<std::string> transitionFrom;
	std::vector<std::string> transitionTo;

	std::vector<std::string> participantStageNames;
	std::vector<std::string> participantRoles;

	std::string assignmentStrategyType;
	std::string routingStrategyType;

	std::vector<std::string> initialDataKeys;
	std::vector<std::string> initialDataValues;

public:
	/**
	 * @brief Destroys the definition and every stage it holds
	 * Releases the WorkItems owned by the workflow definition
	 */
	~WorkflowDefinition();

	/**
	 * @brief Accept a workflow visitor
	 * @param wv Pointer to the WorkflowVisitor
	 */
	void accept(WorkflowVisitor *wv);

	/**
	 * @brief Create a clone of the workflow definition
	 * @return Pointer to the cloned WorkflowDefinition
	 */
	WorkflowDefinition *clone();

	/**
	 * @brief Add a stage to the workflow definition
	 * @param s Pointer to the WorkItem to add as a stage
	 */
	void addStage(WorkItem *s);

	/**
	 * @brief Get the stages of the workflow
	 * @return Pointer to the vector containing the workflow's stages
	 */
	std::vector<WorkItem *> *getStages();

	/**
	 * @brief Set name for workflow
	 * @param name The workflow name
	 */
	void setName(std::string name);

	/**
	 * @brief Get the name of the workflow
	 * @return The name given to the builder
	 */
	std::string getName() const;

	/**
	 * @brief Add a transition from A to B
	 * @param from Name of the starting stage
	 * @param to Name of the destination stage
	 */
	void addTransition(std::string from, std::string to);

	/**
	 * @brief Add participant slot to workflow
	 * @param stageName Name of the stage
	 * @param role Role assigned to the stage
	 */
	void addParticipantSlot(std::string stageName, std::string role);

	/**
	 * @brief Set Assignment Strategy for workflow
	 * @param type Name of the assignment strategy type
	 */
	void setAssignmentStrategyType(std::string type);

	/**
	 * @brief Set Routing Strategy for workflow
	 * @param type Name of the routing strategy type
	 */
	void setRoutingStrategyType(std::string type);

	/**
	 * @brief Get the assignment strategy type the workflow asks for
	 * @return The type name, or empty if none was set
	 */
	std::string getAssignmentStrategyType() const;

	/**
	 * @brief Get the routing strategy type the workflow asks for
	 * @return The type name, or empty if none was set
	 */
	std::string getRoutingStrategyType() const;

	/**
	 * @brief Get the transitions between stages, in the order they were added
	 * @return Pairs of (from stage name, to stage name)
	 */
	std::vector<std::pair<std::string, std::string>> getTransitions() const;

	/**
	 * @brief Add initial data to workflow
	 * @param key Key identifying the data
	 * @param value Value associated with the key
	 */
	void addInitialData(std::string key, std::string value);

	/**
	 * @brief searches and returns the correct stage based on name of the stage
	 * @param stageName Name of the Stage to search for
	 * @return Pointer to the matching Stage, or nullptr if not found
	 */
	Stage *findStage(const std::string &stageName);
};

#endif
