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
	WorkflowDefinition* definition;

public:
	/**
	 * @brief Builds a workflow definition
	 */
	void buildDefinition(std::string name);

	/**
	 * @brief Builds a stage in the workflow
	 */
	void buildStage(std::string name) override;

	/**
	 * @brief Builds a task within a stage in the workflow
	 */
	void buildTask(std::string stageName, std::string title) override;

	/**
	 * @brief Links the workflow handlers
	 */
	void linkHandlers() override;

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
	 * @brief Gets the constructed workflow definition
	 */
	WorkflowDefinition* getWorkflowDefinition();
};

#endif
