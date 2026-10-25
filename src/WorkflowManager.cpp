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
	if (!def)
		return nullptr;

	WorkflowInstance *temp = new WorkflowInstance();

	temp->setId(id); // Add setter to WorkflowInstance
	temp->setDefinition(def);

	// TODO: Why is temp calling attach???????????
	// temp->attach();

	return temp;
}

void WorkflowManager::startWorkflow(WorkflowInstance *instance)
{
	if (!instance)
		return;
	//
}

void WorkflowManager::executeWorkflow(WorkflowInstance *instance, WorkItem *item)
{
	// TODO : This function does not work at all

	// TODO: There s not function called getRootHandlerChain
	/* if (!instance || !item)
		return;

	instance->wi.push_back(item);

	Handler *rootProcessorChain = this->getRootHandlerChain();

	if (rootProcessorChain)
	{
		rootProcessorChain->handleRequest(item, instance);
	} */
}

void WorkflowManager::cancelWorkflow(WorkflowInstance *instance)
{
	if (!instance)
		return;

	/* instance->wi.clear();

	instance->detach(this); */

	// TODO: WorkflowManager is not an observer so it can't detach itself
}

WorkflowManager::~WorkflowManager()
{
	delete this->routingStrategy;
	delete this->assignmentStrategy;
}