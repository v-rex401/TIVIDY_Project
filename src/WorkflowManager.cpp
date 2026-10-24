#include "WorkflowManager.h"

void WorkflowManager::setAssignmentStrategy(AssignmentStrategy *as)
{
	this->assignmentStrategy = as;
}

void WorkflowManager::setRoutingStrategy(RoutingStrategy *rs)
{
	this->routingStrategy = rs;
}

Staff *WorkflowManager::assignWorkItem(WorkItem *wi, WorkflowInstance *wc)
{
	// TODO - implement WorkflowManager::assignWorkItem
	return nullptr;
}

void WorkflowManager::routeWorkItem(WorkItem *wi, WorkflowInstance *wc)
{
	// TODO - implement WorkflowManager::routeWorkItem
	return;
}

WorkflowInstance *WorkflowManager::createInstance(WorkflowDefinition *def)
{
	// TODO - implement WorkflowManager::createInstance
	return nullptr;
}

void WorkflowManager::startWorkflow(WorkflowDefinition *def)
{
	// TODO - implement WorkflowManager::startWorkflow
}

void WorkflowManager::executeWorkflow(WorkflowInstance *i)
{
	// TODO - implement WorkflowManager::executeWorkflow
}

void WorkflowManager::cancelWorkflow(WorkflowInstance *i)
{
	// TODO - implement WorkflowManager::cancelWorkflow
	return;
}
