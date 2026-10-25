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

class Stage : public WorkItem {

private:
	std::string name;
	std::string title;
	std::string requiredPosition;
	std::vector<WorkItem*> members;
	Staff* responsible;

public:
	/**
	 * @brief get the required Staff position of a Stage
	 */
	virtual std::string getRequiredPosition() override;

	/**
	 * @brief set the required Staff position of a Stage
	 */
	virtual void setRequiredPosition(const std::string& position) override;

	/**
	 * @brief Add a work item to the stage
	 */
	void addMember(WorkItem* c);

	/**
	 * @brief Remove a work item from the stage
	 */
	void removeMember(WorkItem* c);

	/**
	 * @brief Handle a workflow request in the chain
	 */
	void handleRequest() override;
	
	/**
	 * @brief Set the next handler in the chain
	 */
	void setNextHandler(WorkItem* next);

	/**
	 * @brief Accept a workflow visitor
	 */
	void accept(WorkflowVisitor* v) override;

	/**
	 * @brief Create a clone of the stage
	 */
	WorkItem* clone() override;

	std::string getName() override;

	std::string getTitle() override;

	/**
	 * @brief Print the stage hierarchy
	 */
	void printHierarchy(std::string indent) override;
};

#endif
