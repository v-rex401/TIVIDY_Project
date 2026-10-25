#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>

class WorkflowVisitor;

/**
 * @brief Abstract base class for tasks and stages in a workflow
 */

class WorkItem {

private:
	WorkItem* successor = nullptr;

public:
	virtual std::string getName() = 0;

	virtual std::string getTitle() = 0;

	virtual std::string getRequiredPosition() = 0;

	virtual void setRequiredPosition(const std::string& position) = 0;

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

	/**
	 * @brief Create a clone of the work item
	 */
	virtual WorkItem* clone() = 0;

	/**
	 * @brief Accept a workflow visitor
	 */
	virtual void accept(WorkflowVisitor* v);

	virtual ~WorkItem() = default;
};

#endif
