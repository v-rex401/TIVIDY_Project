#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <vector>

#include "WorkItem.h"

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
	void handleRequest();
	
	/**
	 * @brief Set the next handler in the chain
	 */
	void setNextHandler(WorkItem* next);

	/**
	 * @brief Accept a workflow visitor
	 */
	void accept(WorkflowVisitor* v);

	/**
	 * @brief Create a clone of the stage
	 */
	WorkItem* clone();

	std::string getName();

	std::string getTitle();

	/**
	 * @brief Print the stage hierarchy
	 */
	void printHierarchy(std::string indent);
};

#endif
