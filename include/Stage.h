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
	std::vector<WorkItem*> members;
	Staff* responsible;

public:
	/**
	 * @brief Construct a stage.
	 * @param name Short identifier for the stage.
	 * @param title Human-readable title shown in reports.
	 *
	 * Both default to empty so clone() can build a blank stage and copy into it.
	 */
	Stage(const std::string& name = "", const std::string& title = "");
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

	void setName(std::string name);
};

#endif
