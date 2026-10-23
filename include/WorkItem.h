#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>

/**
 * @brief Abstract base class for tasks and stages in a workflow
 */

class WorkItem {

private:
	WorkItem* successor;

public:
	virtual std::string getName() = 0;

	virtual std::string getTitle() = 0;

	/**
	 * @brief Print the hierarchy of work items
	 */
	virtual void printHierarchy(std::string indent) = 0;

	/**
	 * @brief Handle a request in the chain
	 */
	virtual void handleRequest() = 0;

	/**
	 * @brief Set the next handler in the chain
	 */
	void setNextHandler(WorkItem* next);

	virtual ~WorkItem() = default;
};

#endif
