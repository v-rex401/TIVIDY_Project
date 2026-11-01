#ifndef HANDLER_H
#define HANDLER_H

#include "WorkItem.h"
#include "Staff.h"
#include <vector>
#include <string>

/**
 * @brief Represents a handler in a chain of responsibility pattern
 */

class Handler : public WorkItem
{

private:
	std::vector<WorkItem *> members;
	std::string name;
	std::string title;
	Staff *responsible;
	std::string severity;
	std::string requiredPosition;

public:
	/**
     * @brief Constructs a Handler with a name and title
     * @param name Short identifier for the Handler
     * @param title Human-readable title of the Handler
     */
	Handler(std::string name, std::string title);

	/**
     * @brief Gets the name of the Handler
     * @return The Handler's name
     */
	std::string getName() override;

	/**
     * @brief Gets the title of the Handler
     * @return The Handler's title
     */
	std::string getTitle() override;

	/**
     * @brief Prints the Handler and its contained WorkItems
     * @param indent String used for indentation
     */
	void printHierarchy(std::string indent) override;

	/**
     * @brief Adds a WorkItem to the Handler
     * @param Component Pointer to the WorkItem to add
     */
	void addMember(WorkItem* Component);

	/**
	 * @brief Removes and deletes a WorkItem from the Handler
	 * @param Component Pointer to the WorkItem to remove and delete
	 */
	void removeMember(WorkItem* Component);

	/**
     * @brief Assigns a responsible Staff member to the Handler
     * Stores a pointer to the Staff member without taking ownership
     * @param staff Pointer to the responsible Staff member
     */
	void setResponsible(Staff* staff) override;

	/**
     * @brief Attempts to handle a workflow request based on Staff rank
     * Delegates the request to the responsible Staff member
     * If the request cannot be handled, it is forwarded to the next WorkItem in the Chain of Responsibility
     * @param rank Minimum Staff rank required
     * @return true if the request was handled, otherwise false
     */
	bool handleRequest(int rank) override;

	/**
	 * @brief Creates a clone of the Handler
	 * Copies the Handler's attributes and clones its contained WorkItems
	 * The responsible Staff pointer is copied without cloning the Staff object
	 * The successor pointer is not copied
	 * @return Pointer to the cloned Handler
	 */
	WorkItem* clone() override;

	/**
     * @brief Gets the severity of the Handler
     * @return The severity level
     */
	std::string getSeverity() override;

	/**
     * @brief Sets the severity of the Handler
     * @param severity The severity label, e.g. "High"
     */
	void setSeverity(const std::string& severity) override;

	 /**
     * @brief Gets the required Staff position of the Handler
     * @return The required Staff position
     */
	std::string getRequiredPosition() override;

	/**
     * @brief Sets the required Staff position of the Handler
     * @param position The required Staff position
     */
	void setRequiredPosition(const std::string& position) override;

	/**
     * @brief Gets the Staff member responsible for the Handler
     * @return Pointer to the responsible Staff member, or nullptr
     */
	Staff* getResponsible() override;

	/**
     * @brief Destroys the Handler
     */
	~Handler() override;
};

#endif
