#ifndef ESCALATINGWORKFLOWBUILDER_H
#define ESCALATINGWORKFLOWBUILDER_H

#include <string>

#include "WorkflowBuilder.h"

class WorkflowDefinition;

/**
 * @brief Concrete builder for constructing escalating workflow definitions
 */

class EscalatingWorkflowBuilder : public WorkflowBuilder {

private:
	WorkflowDefinition* definition = nullptr;

public:
	/**
	 * @brief Destroys a definition that was built but never collected.
	 */
	~EscalatingWorkflowBuilder();
	
	/**
	 * @brief Builds a workflow definition
	 * @param name Name of the workflow definition
	 */
	void buildDefinition(std::string name) override;

	/**
	 * @brief Builds a stage in the workflow
	 * @param name Name of the stage to create
	 */
	void buildStage(std::string name) override;

	/**
	 * @brief Builds a task within a stage in the workflow
	 * @param stageName Name of the stage containing the task
	 * @param title Title of the task to create
	 */
	void buildTask(std::string stageName, std::string title) override;

	/**
	 * @brief Links the workflow handlers
	 */
	void linkHandlers() override;

	/**
	 * @brief Builds a transition between workflow stages
	 * @param from Name of the starting stage
	 * @param to Name of the destination stage
	 */
	void buildTransition(std::string from, std::string to) override;

	/**
	 * @brief Builds a participant slot for a stage in the workflow
	 * @param stageName Name of the stage
	 * @param role Participant role associated with the stage
	 */
	void buildParticipantSlot(std::string stageName, std::string role) override;

	/**
	 * @brief Sets the assignment strategy type for the workflow
	 * @param type Name of the assignment strategy type
	 */
	void setAssignmentStrategyType(std::string type) override;

	/**
	 * @brief Sets the routing strategy type for the workflow
	 * @param type Name of the routing strategy type
	 */
	void setRoutingStrategyType(std::string type) override;

	/**
	 * @brief Sets the initial data for the workflow
	 * @param key Key identifying the initial data
	 * @param value Value associated with the key
	 */
	void setInitialData(std::string key, std::string value) override;

	/**
	 * @brief Gets the constructed workflow definition
	 * @return Pointer to the constructed WorkflowDefinition, or nullptr if no definition is available
	 */
	WorkflowDefinition* getWorkflowDefinition() override;
};

#endif
