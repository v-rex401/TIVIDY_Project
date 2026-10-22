#ifndef WORKFLOWDIRECTOR_H
#define WORKFLOWDIRECTOR_H

#include <string>
#include <vector>
#include <map>

class WorkflowBuilder;

class WorkflowDirector {

private:
	WorkflowBuilder* builder;

public:
	WorkflowDirector(WorkflowBuilder* b);

	void construct(std::string name, std::vector<std::string> stages, std::map<std::string, std::vector<std::string>> tasks);
};

#endif
