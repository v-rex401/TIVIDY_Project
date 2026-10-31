#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>

class WorkflowVisitor;
class Staff;

/**
 * @brief Abstract base class for tasks and stages in a workflow
 */

class WorkItem {

private:
	WorkItem* successor = nullptr;

public:
    /**
     * @brief Get the next handler in the chain
     * @return The successor, or nullptr at the end of the chain.
     *
     * Public so a client and a cloned definition can follow the chain.
     */
    WorkItem* getNextHandler() const
    {
        return this->successor;
    }

	virtual std::string getName() = 0;

	virtual std::string getTitle() = 0;

	/**
	 * @brief get the severity of a Stage
	 */
	virtual std::string getSeverity() = 0;

	/**
	 * @brief Set how urgent the item is, e.g. "Low", "High" or "Critical".
	 *
	 * SeverityBasedRouting reads this to pick the emergency stage. The default
	 * does nothing, for work items that carry no severity.
	 *
	 * @param severity The severity label.
	 */
	virtual void setSeverity(const std::string& severity) { (void)severity; }

	/**
	 * @brief get the required Staff position of a Stage
	 */
	virtual std::string getRequiredPosition() = 0;

	/**
	 * @brief set the required Staff position of a Stage
	 */
	virtual void setRequiredPosition(const std::string& position) = 0;

	/**
	 * @brief Print the hierarchy of work items
	 */
	virtual void printHierarchy(std::string indent) = 0;

	/**
	 * @brief Attempts to handle a request based on the required staff rank
	 * @param rank Minimum staff rank required to handle the request
	 * @return true if the request was handled, otherwise false
	 */
	virtual bool handleRequest(int rank) = 0;

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

	virtual Staff* getResponsible() { return nullptr; }
	virtual void setResponsible(Staff*) {}

	virtual ~WorkItem() = default;
};

#endif
