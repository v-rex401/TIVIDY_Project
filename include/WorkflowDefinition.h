#ifndef WORKFLOWDEFINITION_H
#define WORKFLOWDEFINITION_H

#include <string>
#include <vector>

class WorkItem;
class WorkflowVisitor;
class WorkflowBuilder;

class WorkflowDefinition {

private:
	std::string name;
	std::vector<WorkItem*> stages;

public:
	void accept(WorkflowVisitor* wv);

	WorkflowDefinition* clone();

	void addStage(WorkItem* s);

	std::vector<WorkItem*>* getStages();

	void setBuilder(WorkflowBuilder* b);
};

#endif
