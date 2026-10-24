#ifndef WORKFLOWMANAGER_H
#define WORKFLOWMANAGER_H

#include "AssignmentStrategy.h"
#include "RoutingStrategy.h"

class WorkflowManager
{

private:
	AssignmentStrategy *assignmentStrategy;
	RoutingStrategy *routingStrategy;

public:
	void setAssignmentStrategy(AssignmentStrategy *as);

	void setRoutingStrategy(RoutingStrategy *rs);

	Staff *assignWorkItem(WorkItem *wi, WorkflowInstance *wc);

	Stage *routeWorkItem(WorkItem *wi, WorkflowInstance *wc);

	WorkflowInstance *createInstance(WorkflowDefinition *def, std::string id);

	void startWorkflow(WorkflowInstance *instance);

	void executeWorkflow(WorkflowInstance *instance, WorkItem *item);

	void cancelWorkflow(WorkflowInstance *i);

	~WorkflowManager();
};

#endif
