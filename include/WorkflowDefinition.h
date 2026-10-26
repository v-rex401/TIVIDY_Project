#ifndef WORKFLOWDEFINITION_H
#define WORKFLOWDEFINITION_H

#include <string>
#include <vector>

class WorkItem;
class WorkflowVisitor;

/**
 * @brief Defines the structure and stages of a workflow
 */

class WorkflowDefinition {

private:
	std::string name;
	std::vector<WorkItem*> stages;
	std::vector<std::string> transitionFrom;
	std::vector<std::string> transitionTo;

public:
	/**
	 * @brief Accept a workflow visitor
	 */
	void accept(WorkflowVisitor* wv);

	/**
	 * @brief Create a clone of the workflow definition
	 */
	WorkflowDefinition* clone();

	/**
	 * @brief Add a stage to the workflow definition
	 */
	void addStage(WorkItem* s);

	/**
	 * @brief Get the stages of the workflow
	 */
	std::vector<WorkItem*>* getStages();

	/**
	 * @brief Set name for workflow
	 */
	void setName(std::string name);

	/**
	 * @brief Add a transition from A to B
	 */
	void addTransition(std::string from, std::string to);

};

#endif
