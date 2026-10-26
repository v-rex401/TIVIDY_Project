#ifndef SEQUENTIALWORKFLOWBUILDER_H
#define SEQUENTIALWORKFLOWBUILDER_H

#include <string>

#include "WorkflowBuilder.h"

class WorkflowDefinition;

/**
 * @brief Concrete builder for constructing sequential workflow definitions
 */

class SequentialWorkflowBuilder : public WorkflowBuilder {

private:
	WorkflowDefinition* definition = nullptr;

public:
	/**
	 * @brief Builds a workflow definition
	 */
	void buildDefinition(std::string name) override;

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
	void buildTransition(std::string from, std::string to) override;

	/**
	 * @brief Builds a participant slot for a stage in the workflow
	 */
	void buildParticipantSlot(std::string stageName, std::string role) override;

	/**
	 * @brief Sets the assignment strategy that the workflow should use
	 */
	void setAssignmentStrategyType(std::string type) override;

	/**
	 * @brief Sets the routing strategy that the workflow should use
	 */
	void setRoutingStrategyType(std::string type) override;

	/**
	 * @brief Stores key data values for the workflow
	 */
	void setInitialData(std::string key, std::string value) override;

	/**
	 * @brief Retrieves the constructed workflow definition
	 */
	WorkflowDefinition* getWorkflowDefinition() override;
};

#endif
