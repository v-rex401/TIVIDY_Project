#ifndef WORKFLOWFACADE_H
#define WORKFLOWFACADE_H

class WorkflowFacade {

private:
	WorkflowManager* manager;

public:
	void startWorkflow(int definition);

	void createWorkflowInstance(int definition);

	void submitWorkItem(int instance, int workitem);

	void executeWorkflow(WorkflowInstance* w);

	void cancelWorkflow(WorkflowInstance* instance);
};

#endif
