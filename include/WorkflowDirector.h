#ifndef WORKFLOWDIRECTOR_H
#define WORKFLOWDIRECTOR_H

#include <string>
#include <vector>
#include <map>

class WorkflowBuilder;

/**
 * @brief Director participant of the Builder pattern.
 *
 * Owns the construction algorithm and issues buildX() calls to a
 * WorkflowBuilder in the correct order
 * The builder is referenced, not owned
 */
class WorkflowDirector {

private:
	WorkflowBuilder* builder;

public:
	/**
	 * @brief Construct a director that drives the given builder.
	 * @param b The builder to issue calls to. Not owned by the director.
	 */
	WorkflowDirector(WorkflowBuilder* b);

	/**
	 * @brief Constructs a workflow with the specified stages and tasks
	 */
	void construct(std::string name, std::vector<std::string> stages, std::map<std::string, std::vector<std::string>> tasks);
};

#endif
