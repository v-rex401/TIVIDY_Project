#include "WorkflowManager.h"

void WorkflowManager::setAssignmentStrategy(AssignmentStrategy* as) {
	this->assignmentStrategy = as;
}

void WorkflowManager::setRoutingStrategy(RoutingStrategy* rs) {
	this->routingStrategy = rs;
}

Staff* WorkflowManager::assignWorkItem(WorkItem* wi, WorkflowInstance* wc) {
	// TODO - implement WorkflowManager::assignWorkItem
	throw "Not yet implemented";
}

void WorkflowManager::routeWorkItem(WorkItem wi, WorkflowInstance* wc) {
	// TODO - implement WorkflowManager::routeWorkItem
	throw "Not yet implemented";
}

WorkflowInstance* WorkflowManager::createInstance(WorkflowDefinition* def) {
	// TODO - implement WorkflowManager::createInstance
	throw "Not yet implemented";
}

void WorkflowManager::startWorkflow(WorkflowDefinition* def) {
	// TODO - implement WorkflowManager::startWorkflow
	throw "Not yet implemented";
}

void WorkflowManager::executeWorkflow(WorkflowInstance* i) {
	// TODO - implement WorkflowManager::executeWorkflow
	throw "Not yet implemented";
}

void WorkflowManager::cancelWorkflow(WorkflowInstance* i) {
	// TODO - implement WorkflowManager::cancelWorkflow
	throw "Not yet implemented";
}
