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
	 * @param name Name of the stage to create
	 */
	virtual void buildStage(std::string name) = 0;

	/**
	 * @brief Builds a task within a stage in the workflow
	 * @param stageName Name of the stage containing the task
	 * @param title Title of the task to create
	 */
	virtual void buildTask(std::string stageName, std::string title) = 0;

	/**
	 * @brief Links the workflow handlers
	 */
	virtual void linkHandlers() = 0;

	/**
	 * @brief Builds a transition between workflow stages
	 * @param from Name of the starting stage
	 * @param to Name of the destination stage
	 */
	virtual void buildTransition(std::string from, std::string to) = 0;

	/**
	 * @brief Builds a participant slot for a stage in the workflow
	 * @param stageName Name of the stage
	 * @param role Participant role associated with the stage
	 */
	virtual void buildParticipantSlot(std::string stageName, std::string role) = 0;

	/**
	 * @brief Sets the assignment strategy type for the workflow
	 * @param type Name of the assignment strategy type
	 */
	virtual void setAssignmentStrategyType(std::string type) = 0;

	/**
	 * @brief Sets the routing strategy type for the workflow
	 * @param type Name of the routing strategy type
	 */
	virtual void setRoutingStrategyType(std::string type) = 0;

	/**
	 * @brief Sets the initial data for the workflow
	 * @param key Key identifying the initial data
	 * @param value Value associated with the key
	 */
	virtual void setInitialData(std::string key, std::string value) = 0;

	/**
	 * @brief Builds a workflow definition
	 * @param name Name of the workflow definition
	 */
	virtual void buildDefinition(std::string name) = 0;

	/**
	 * @brief Returns workflow definition
	 * @return Pointer to the constructed WorkflowDefinition
	 */
	virtual WorkflowDefinition* getWorkflowDefinition() = 0;

	/**
     * @brief Virtual destructor for the WorkflowBuilder interface
     * Ensures proper destruction of concrete builder objects when deleted through a WorkflowBuilder pointer
     */
	virtual ~WorkflowBuilder()  = default;
};

#endif
