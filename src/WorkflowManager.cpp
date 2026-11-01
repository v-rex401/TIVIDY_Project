#include "WorkflowManager.h"
#include "Cancelled.h"
#include <algorithm>

WorkflowManager::WorkflowManager() : assignmentStrategy(nullptr), routingStrategy(nullptr), externalSystem(nullptr) {}

void WorkflowManager::setExternalSystem(ExternalSystem *es)
{
	this->externalSystem = es;
}

void WorkflowManager::notifyExternal()
{
	if (this->externalSystem != nullptr)
	{
		this->externalSystem->request();
	}
}

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

	Staff *assignee = this->assignmentStrategy->selectAssignmentItem(wi, wc, runningInstances);
	
	if(assignee != nullptr){
		// Nobody took the work so there is nothing to tell the outside system
		this->notifyExternal();
	}

	return assignee;
}

Stage *WorkflowManager::routeWorkItem(WorkItem *wi, WorkflowInstance *wc)
{
	if (routingStrategy == nullptr || wi == nullptr || wc == nullptr) return nullptr;

	Stage *next = this->routingStrategy->selectItemState(wi, wc);
	if(next != nullptr){
		//Nothing was routed, so there is nothing to tell the outside system
		this->notifyExternal();
	}

	return next;
}

WorkflowInstance *WorkflowManager::createInstance(WorkflowDefinition *def, std::string id)
{
	if (!def)
		return nullptr;

	WorkflowInstance *temp = new WorkflowInstance();

	temp->setId(id); // Add setter to WorkflowInstance
	temp->setDefinition(def);

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

	instance->handle();
}

void WorkflowManager::executeWorkflow(WorkflowInstance *instance, WorkItem *item)
{
	if (instance == nullptr || item == nullptr) return;

	Staff* assignedStaff = assignWorkItem(item, instance);
	if (assignedStaff == nullptr) return;

	Stage* destination = routeWorkItem(item, instance);
	if (destination == nullptr) return;

	item->setResponsible(assignedStaff);
	destination->addMember(item);
	instance->addWorkItem(item);
	instance->addParticipant(assignedStaff);
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