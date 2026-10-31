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
	/**
	 * @brief Construtor for a Task
	 */
	Task(const std::string& name = "", const std::string& title = "");

	/**
	 * @brief Getter for the responsible Staff
	 */
	Staff* getResponsible() override;

	/**
	 * @brief Setter for the resposnible Staff
	 */
	void setResponsible(Staff* staff) override;
	
	/**
	 * @brief Getter for the title
	 */
	std::string getName() override;

	/**
	 * @brief Getter for the title
	 */
	std::string getTitle() override;

	/**
	 * @brief get the severity of a Task
	 */
	std::string getSeverity() override;

	/**
	 * @brief set the severity of a Task
	 * @param severity The severity label, e.g. "High".
	 */
	void setSeverity(const std::string& severity) override;

	/**
	 * @brief get the required Staff position of a Task
	 */
	std::string getRequiredPosition() override;

	/**
	 * @brief set the required Staff position of a Task
	 */
	void setRequiredPosition(const std::string& position) override;

	/**
	 * @brief Print the tasks in a workflow hierarchy
	 */
	void printHierarchy(std::string indent) override;

	/**
	 * @brief Attempts to handle a workflow request based on required staff rank
	 * @param rank Minimum staff rank required
	 * @return true if handled, otherwise false
	 */
	bool handleRequest(int rank) override;

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
