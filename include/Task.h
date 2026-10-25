#ifndef TASK_H
#define TASK_H

#include <string>

#include "WorkItem.h"

class Staff;
class WorkflowVisitor;

/**
 * @brief Represents a task in a workflow
 */

class Task : public WorkItem
{

private:
	std::string name;
	std::string title;
	std::string requiredPosition;
	std::string severity;
	Staff *responsible;

public:
	Staff* getResponsible() override;
	void setResponsible(Staff* staff) override;
	
	std::string getName() override;

	std::string getTitle() override;

	/**
	 * @brief get the severity of a Stage
	 */
	virtual std::string getSeverity() override;

	/**
	 * @brief get the required Staff position of a Stage
	 */
	virtual std::string getRequiredPosition() override;

	/**
	 * @brief set the required Staff position of a Stage
	 */
	virtual void setRequiredPosition(const std::string& position) override;

	/**
	 * @brief Print the tasks in a workflow hierarchy
	 */
	void printHierarchy(std::string indent) override;

	/**
	 * @brief Handle a workflow request in the chain
	 */
	void handleRequest() override;

	/**
	 * @brief Set the next handler in the chain
	 */
	void setNextHandler(WorkItem *next);

	/**
	 * @brief Create a clone of the task
	 */
	WorkItem *clone() override;

	/**
	 * @brief Accept a workflow visitor
	 */
	void accept(WorkflowVisitor *v) override;
};

#endif
