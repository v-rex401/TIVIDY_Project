#ifndef WORKFLOWDIRECTOR_H
#define WORKFLOWDIRECTOR_H

#include <string>
#include <vector>
#include <map>

class WorkflowBuilder;

/**
 * @brief Constructs a workflow director with a builder
 */

class WorkflowDirector {

private:
	WorkflowBuilder* builder;

public:
	WorkflowDirector(WorkflowBuilder* b);

	/**
	 * @brief Constructs a workflow with the specified stages and tasks
	 */
	void construct(std::string name, std::vector<std::string> stages, std::map<std::string, std::vector<std::string>> tasks);
};

#endif
