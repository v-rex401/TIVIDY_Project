#include "WorkflowManager.h"
#include "Cancelled.h"
#include <algorithm>

WorkflowManager::WorkflowManager() : assignmentStrategy(nullptr), routingStrategy(nullptr) {}

void WorkflowManager::setAssignmentStrategy(AssignmentStrategy *as)
{
	if (assignmentStrategy == as) {
		return;
	}

	delete assignmentStrategy;
	this->assignmentStrategy = as;
}

void WorkflowManager::setRoutingStrategy(RoutingStrategy *rs)
{
	if (routingStrategy == rs) {
		return;
	}

	delete routingStrategy;
	this->routingStrategy = rs;
}

Staff *WorkflowManager::assignWorkItem(WorkItem *wi, WorkflowInstance *wc)
{
	if (assignmentStrategy == nullptr || wi == nullptr || wc == nullptr) return nullptr;
	return this->assignmentStrategy->selectAssignmentItem(wi, wc, runningInstances);
}

Stage *WorkflowManager::routeWorkItem(WorkItem *wi, WorkflowInstance *wc)
{
	if (assignmentStrategy == nullptr || wi == nullptr || wc == nullptr) return nullptr;
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
	if (instance == nullptr) return;

	auto it = std::find(runningInstances.begin(), runningInstances.end(), instance);

	if (it != runningInstances.end()) {
		return;
	}

	runningInstances.push_back(instance);
}

void WorkflowManager::executeWorkflow(WorkflowInstance *instance, WorkItem *item)
{
	if (instance == nullptr || item == nullptr) return;

	//
}

void WorkflowManager::cancelWorkflow(WorkflowInstance *instance)
{
	if (instance == nullptr) return;

	auto it = std::find(runningInstances.begin(), runningInstances.end(), instance);

	if (it == runningInstances.end()) {
		return;
	}

	instance->setState(new Cancelled());

	runningInstances.erase(it);
}

WorkflowManager::~WorkflowManager()
{
	delete this->routingStrategy;
	delete this->assignmentStrategy;
}