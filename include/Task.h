#ifndef TASK_H
#define TASK_H

#include <string>

#include "WorkItem.h"
#include "Staff.h"

/**
 * @brief Represents a task in a workflow
 */

class Task : public WorkItem {

private:
	std::string name;
	std::string title;
	Staff responsible;

public:
	std::string getName();

	std::string getTitle();

	/**
	 * @brief Print the tasks in a workflow hierarchy
	 */
	void printHierarchy(std::string indent);

	/**
	 * @brief Handle a workflow request in the chain
	 */
	void handleRequest();

	/**
	 * @brief Set the next handler in the chain
	 */
	void setNextHandler(WorkItem* next);
};

#endif
