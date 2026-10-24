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
	return this->assignmentStrategy->selectAssignmentItem(wi, wc);
}

Stage *WorkflowManager::routeWorkItem(WorkItem *wi, WorkflowInstance *wc)
{
	return this->routingStrategy->selectItemState(wi, wc);
}

WorkflowInstance *WorkflowManager::createInstance(WorkflowDefinition *def, std::string id)
{
	if (!def) return nullptr;

	WorkflowInstance* temp = new WorkflowInstance();

	temp->setId(id); //Add setter to WorkflowInstance
	temp->setDefinition(def); 

	temp->attach();

	return temp;
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

WorkflowManager::~WorkflowManager() {
	delete this->routingStrategy;
	delete this->assignmentStrategy;
}