#ifndef WORKFLOWMANAGER_H
#define WORKFLOWMANAGER_H

#include "AssignmentStrategy.h"
#include "RoutingStrategy.h"
#include "ExternalSystem.h"

class WorkflowManager
{

private:
	AssignmentStrategy *assignmentStrategy;
	RoutingStrategy *routingStrategy;
	ExternalSystem *externalSystem;

public:

	/** @brief Starts with no strategies and no external system configured. */
	WorkflowManager();

	void setAssignmentStrategy(AssignmentStrategy *as);

	void setRoutingStrategy(RoutingStrategy *rs);

	void setExternalSystem(ExternalSystem *es);

	Staff *assignWorkItem(WorkItem *wi, WorkflowInstance *wc);

	Stage *routeWorkItem(WorkItem *wi, WorkflowInstance *wc);

	WorkflowInstance *createInstance(WorkflowDefinition *def, std::string id);

	void startWorkflow(WorkflowInstance *instance);

	void executeWorkflow(WorkflowInstance *instance, WorkItem *item);

	void cancelWorkflow(WorkflowInstance *i);

	~WorkflowManager();

	void notifyExternal();
};

#endif
