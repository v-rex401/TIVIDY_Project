#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <vector>

#include "WorkItem.h"

class Staff;
class WorkflowVisitor;
/**
 * @brief Represents a stage (that contains work items) in a workflow
 */

class Stage : public WorkItem
{

private:
	std::string name;
	std::string title;
	std::string requiredPosition;
	std::string severity;
	std::vector<WorkItem *> members;
	Staff *responsible;

public:
	/**
	 * @brief get the responsible staff member in a stage
	 * @return Pointer to the responsible Staff member, or nullptr
	 */
	Staff *getResponsible() override;

	/**
	 * @brief set the responsible staff member in a stage
	 * @param staff Pointer to the responsible Staff member
	 */
	void setResponsible(Staff *staff) override;

	/**
	 * @brief get the severity of a Stage
	 * @return The severity level
	 */
	virtual std::string getSeverity() override;

	/**
	 * @brief set the severity of a Stage
	 * @param severity The severity label, e.g. "High".
	 */
	void setSeverity(const std::string &severity) override;

	/**
	 * @brief get the required Staff position of a Stage
	 * @return The required Staff position
	 */
	virtual std::string getRequiredPosition() override;

	/**
	 * @brief set the required Staff position of a Stage
	 * @param position The required Staff position
	 */
	virtual void setRequiredPosition(const std::string &position) override;

	/**
	 * @brief Construct a stage.
	 * @param name Short identifier for the stage.
	 * @param title Human-readable title shown in reports.
	 *
	 * Both default to empty so clone() can build a blank stage and copy into it.
	 */
	Stage(const std::string &name = "", const std::string &title = "");

	/**
	 * @brief Add a work item to the stage
	 * @param c Pointer to the WorkItem to add
	 */
	void addMember(WorkItem *c);

	/**
	 * @brief Getter for all workitems
	 * @return Constant reference to the collection of WorkItems
	 */
	const std::vector<WorkItem *> &getMembers() const;

	/**
	 * @brief Removes a WorkItem from the Stage without deleting it
	 * @param c Pointer to the WorkItem to remove
	 */
	void removeMember(WorkItem *c);

	/**
	 * @brief Attempts to handle a workflow request based on required staff rank
	 * @param rank Minimum staff rank required to handle the request
	 * @return true if the request was handled, otherwise false
	 */
	bool handleRequest(int rank) override;

	/**
	 * @brief Set the next handler in the chain
	 * @param next Pointer to the next WorkItem
	 */
	void setNextHandler(WorkItem *next);

	/**
	 * @brief Accept a workflow visitor
	 * @param v Pointer to the visitor
	 */
	void accept(WorkflowVisitor *v) override;

	/**
	 * @brief Create a clone of the stage
	 * @return Pointer to the cloned Stage
	 */
	WorkItem *clone() override;

	/**
     * @brief Gets the name of the Stage
     * @return The Stage's name
     */
	std::string getName() override;

	/**
     * @brief Sets the name of the Stage
     * @param name The new name
     */
	void setName(std::string name);

	/**
     * @brief Gets the title of the Stage
     * @return The Stage's title
     */
	std::string getTitle() override;

	/**
	 * @brief Print the stage hierarchy
	 * @param indent String used for indentation
	 */
	void printHierarchy(std::string indent) override;

	/**
     * @brief Destroys the Stage and its owned WorkItems
     */
	~Stage();
};

#endif
