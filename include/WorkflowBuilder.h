#ifndef WORKFLOWBUILDER_H
#define WORKFLOWBUILDER_H

#include <string>

class WorkflowDefinition;

/**
 * @brief Abstract builder for constructing workflow definitions
 */

class WorkflowBuilder {


public:
	/**
	 * @brief Builds a stage in the workflow
	 */
	virtual void buildStage(std::string name) = 0;

	/**
	 * @brief Builds a task within a stage in the workflow
	 */
	virtual void buildTask(std::string stageName, std::string title) = 0;

	/**
	 * @brief Links the workflow handlers
	 */
	virtual void linkHandlers() = 0;

	/**
	 * @brief Builds a transition between workflow stages
	 */
	virtual void buildTransition(std::string from, std::string to) = 0;

	/**
	 * @brief Builds a participant slot for a stage in the workflow
	 */
	virtual void buildParticipantSlot(std::string stageName, std::string role) = 0;

	/**
	 * @brief Sets the assignment strategy type for the workflow
	 */
	virtual void setAssignmentStrategyType(std::string type) = 0;

	/**
	 * @brief Sets the routing strategy type for the workflow
	 */
	virtual void setRoutingStrategyType(std::string type) = 0;

	/**
	 * @brief Sets the initial data for the workflow
	 */
	virtual void setInitialData(std::string key, std::string value) = 0;

	/**
	 * @brief Builds a workflow definition
	 */
	virtual void buildDefinition(std::string name) = 0;

	/**
	 * @brief Returns workflow definition
	 */
	virtual WorkflowDefinition* getWorkflowDefinition() = 0;

	virtual ~WorkflowBuilder()  = default;
};

#endif
