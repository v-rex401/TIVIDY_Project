#ifndef WORKFLOWBUILDER_H
#define WORKFLOWBUILDER_H

#include <string>

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
	void buildTransition(std::string from, std::string to);

	/**
	 * @brief Builds a participant slot for a stage in the workflow
	 */
	void buildParticipantSlot(std::string stageName, std::string role);

	/**
	 * @brief Sets the assignment strategy type for the workflow
	 */
	void setAssignmentStrategyType(std::string type);

	/**
	 * @brief Sets the routing strategy type for the workflow
	 */
	void setRoutingStrategyType(std::string type);

	/**
	 * @brief Sets the initial data for the workflow
	 */
	void setInitialData(std::string key, std::string value);

	/**
	 * @brief Builds a workflow definition
	 */
	void buildDefinition(std::string name);

	virtual ~WorkflowBuilder()  = default;
};

#endif
