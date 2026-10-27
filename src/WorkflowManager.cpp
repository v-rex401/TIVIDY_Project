#include "WorkflowManager.h"

WorkflowManager::WorkflowManager(){
	this->assignmentStrategy = nullptr;
	this->routingStrategy = nullptr;
	this->externalSystem = nullptr;
}

void WorkflowManager::setAssignmentStrategy(AssignmentStrategy *as)
{
	this->assignmentStrategy = as;
}

void WorkflowManager::setRoutingStrategy(RoutingStrategy *rs)
{
	this->routingStrategy = rs;
}

void WorkflowManager::setExternalSystem(ExternalSystem *es)
{
	this->externalSystem = es;
}

void WorkflowManager::notifyExternal()
{
	if (this->externalSystem != nullptr){
		this->externalSystem->request();
	}
}

Staff *WorkflowManager::assignWorkItem(WorkItem *wi, WorkflowInstance *wc)
{
	if (this->assignmentStrategy == nullptr){
		return nullptr;
	}

	Staff *assignee = this->assignmentStrategy->selectAssignmentItem(wi, wc);
	this->notifyExternal();

	return assignee;
}

Stage *WorkflowManager::routeWorkItem(WorkItem *wi, WorkflowInstance *wc){
	if (this->routingStrategy == nullptr){
		return nullptr;
	}

	Stage *next = this->routingStrategy->selectItemState(wi, wc);
	this->notifyExternal();

	return next;
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

	// TODO: There is not function called getRootHandlerChain
	if (!instance || !item)
		return;

	/*
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