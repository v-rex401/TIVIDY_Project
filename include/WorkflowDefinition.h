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
	 * @brief Destroys the definition and every stage it holds.
	 */
	~WorkflowDefinition();
	/**
	 * @brief Accept a workflow visitor
	 */
	void accept(WorkflowVisitor *wv);

	/**
	 * @brief Create a clone of the workflow definition
	 */
	WorkflowDefinition *clone();

	/**
	 * @brief Add a stage to the workflow definition
	 */
	void addStage(WorkItem *s);

	/**
	 * @brief Get the stages of the workflow
	 */
	std::vector<WorkItem *> *getStages();

	/**
	 * @brief Set name for workflow
	 */
	void setName(std::string name);

	/**
	 * @brief Get the name of the workflow
	 * @return The name given to the builder.
	 */
	std::string getName() const;

	/**
	 * @brief Add a transition from A to B
	 */
	void addTransition(std::string from, std::string to);

	/**
	 * @brief Add participant slot to workflow
	 */
	void addParticipantSlot(std::string stageName, std::string role);

	/**
	 * @brief Set Assignment Strategy for workflow
	 */
	void setAssignmentStrategyType(std::string type);

	/**
	 * @brief Set Routing Strategy for workflow
	 */
	void setRoutingStrategyType(std::string type);

	/**
	 * @brief Get the assignment strategy type the workflow asks for
	 * @return The type name, or empty if none was set.
	 */
	std::string getAssignmentStrategyType() const;

	/**
	 * @brief Get the routing strategy type the workflow asks for
	 * @return The type name, or empty if none was set.
	 */
	std::string getRoutingStrategyType() const;

	/**
	 * @brief Get the transitions between stages, in the order they were added
	 * @return Pairs of (from stage name, to stage name).
	 */
	std::vector<std::pair<std::string, std::string>> getTransitions() const;

	/**
	 * @brief Add initial data to workflow
	 */
	void addInitialData(std::string key, std::string value);

	/**
	 * @brief searches and returns the correct stage based on name of the stage
	 */
	Stage *findStage(const std::string &stageName);
};

#endif
