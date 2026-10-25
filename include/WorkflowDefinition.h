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
	 * @brief Set the workflow definition name
	 */
	void setName(std::string name);

	~WorkflowDefinition();
};

#endif
