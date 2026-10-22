#ifndef WORKFLOWDIRECTOR_H
#define WORKFLOWDIRECTOR_H

class WorkflowDirector {

private:
	WorkflowBuilder* builder;

public:
	WorkflowDirector(WorkflowBuilder* b);

	void construct(string name, vector<string> stages, map<string, vector<string>> tasks);
};

#endif
