/**
 * @file WorkflowFacade.cpp
 * @brief Implementation of WorkflowFacade: every call is delegated to the WorkflowManager.
 * @author JP
 * @date 2026-10-24
 */

#include "WorkflowFacade.h"
#include "WorkflowManager.h"

WorkflowFacade::WorkflowFacade() : manager(new WorkflowManager()) {}

WorkflowFacade::~WorkflowFacade() {
	delete manager;
}

void WorkflowFacade::startWorkflow(WorkflowDefinition* definition) {
	manager->startWorkflow(definition);
}

WorkflowInstance* WorkflowFacade::createWorkflowInstance(WorkflowDefinition* definition) {
	return manager->createInstance(definition);
}

void WorkflowFacade::submitWorkItem(WorkflowInstance* instance, WorkItem* workitem) {
	manager->assignWorkItem(workitem, instance);
	manager->routeWorkItem(workitem, instance);
}

void WorkflowFacade::executeWorkflow(WorkflowInstance* w) {
	manager->executeWorkflow(w);
}

void WorkflowFacade::cancelWorkflow(WorkflowInstance* instance) {
	manager->cancelWorkflow(instance);
}

void WorkflowFacade::addDefinition(WorkflowDefinition* definition) {
	manager->addDefinition(definition);
}

std::vector<WorkflowDefinition*> WorkflowFacade::getDefinitions() {
	return manager->getDefinitions();
}

std::vector<WorkflowInstance*> WorkflowFacade::getInstances() {
	return manager->getInstances();
}

WorkflowInstance* WorkflowFacade::getInstance(const std::string& id) {
	return manager->getInstance(id);
}
