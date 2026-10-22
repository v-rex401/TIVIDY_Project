#ifndef WORKFLOWDEFINITION_H
#define WORKFLOWDEFINITION_H

class WorkflowDefinition {

private:
	string name;
	vector<WorkItem*> stages;

public:
	void accept(WorkflowVisitor* wv);

	WorkflowDefinition* clone();

	void addStage(WorkItem* s);

	vector<WorkItem*>* getStages();

	void setBuilder(WorkflowBuilder* b);
};

#endif
