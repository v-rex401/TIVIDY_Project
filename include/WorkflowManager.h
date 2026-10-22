#ifndef WORKFLOWMANAGER_H
#define WORKFLOWMANAGER_H

class WorkflowManager {

private:
	AssignmentStrategy* assignmentStrategy;
	RoutingStrategy* routingStrategy;

public:
	void setAssignmentStrategy(AssignmentStrategy* as);

	void setRoutingStrategy(RoutingStrategy* rs);

	Staff* assignWorkItem(WorkItem* wi, WorkflowInstance* wc);

	void routeWorkItem(WorkItem wi, WorkflowInstance* wc);

	WorkflowInstance* createInstance(WorkflowDefinition* def);

	void startWorkflow(WorkflowDefinition* def);

	void executeWorkflow(WorkflowInstance* i);

	void cancelWorkflow(WorkflowInstance* i);
};

#endif
