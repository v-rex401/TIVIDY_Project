#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>

class WorkflowVisitor;
class Staff;

/**
 * @brief Abstract base class for workflow items
 * 
 * Defines the common interface for Tasks, Stages and Handlers
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

	/**
     * @brief Gets the name of the WorkItem
     * @return The WorkItem's name
     */
	virtual std::string getName() = 0;

	/**
     * @brief Gets the title of the WorkItem
     * @return The WorkItem's title
     */
	virtual std::string getTitle() = 0;

	/**
	 * @brief get the severity of the WorkItem
	 * @return The severity level
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
	 * @brief get the required Staff position of the WorkItem
	 * @return The required Staff position
	 */
	virtual std::string getRequiredPosition() = 0;

	/**
	 * @brief set the required Staff position of the WorkItem
	 * @param position The required Staff position
	 */
	virtual void setRequiredPosition(const std::string& position) = 0;

	/**
	 * @brief Print the hierarchy of work items
	 * @param indent String used for indentation
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
	 * @param next Pointer to the next WorkItem
	 */
	void setNextHandler(WorkItem* next);

	/**
	 * @brief Create a clone of the work item
	 * @return Pointer to the cloned WorkItem
	 */
	virtual WorkItem* clone() = 0;

	/**
	 * @brief Accept a workflow visitor
	 * @param v Pointer to the visitor
	 */
	virtual void accept(WorkflowVisitor* v);
	
	/**
     * @brief Gets the Staff member responsible for the WorkItem
     * @return Pointer to the responsible Staff member, or nullptr if none is assigned
     */
	virtual Staff* getResponsible() { return nullptr; }

	/**
     * @brief Assigns a responsible Staff member to the WorkItem
     * The default implementation performs no action
     * @param staff Pointer to the Staff member
     */
	virtual void setResponsible(Staff*) {}

	/**
     * @brief Virtual destructor for the WorkItem base class.
     */
	virtual ~WorkItem() = default;
};

#endif
