/**
 * @file WorkflowFacade.cpp
 * @brief Implementation of WorkflowFacade: every call is delegated to the WorkflowManager.
 * @author JP
 * @date 2026-10-24
 */

#include "WorkflowFacade.h"
#include "WorkflowManager.h"

WorkflowFacade::WorkflowFacade() : manager(new WorkflowManager()) {}

WorkflowFacade::~WorkflowFacade()
{
	delete manager;
}

void WorkflowFacade::startWorkflow(WorkflowDefinition *definition)
{
	manager->startWorkflow(definition);
}

WorkflowInstance *WorkflowFacade::createWorkflowInstance(WorkflowDefinition *definition)
{
	return manager->createInstance(definition);
}

void WorkflowFacade::submitWorkItem(WorkflowInstance *instance, WorkItem *workitem)
{
	manager->assignWorkItem(workitem, instance);
	manager->routeWorkItem(workitem, instance);
}

void WorkflowFacade::executeWorkflow(WorkflowInstance *w)
{
	manager->executeWorkflow(w);
}

void WorkflowFacade::cancelWorkflow(WorkflowInstance *instance)
{
	manager->cancelWorkflow(instance);
}

void WorkflowFacade::addDefinition(WorkflowDefinition *definition)
{
	// manager->addDefinition(definition);
	manager->createInstance(definition);
}

std::vector<WorkflowDefinition *> WorkflowFacade::getDefinitions()
{
	// TODO : Add method in WorkflowManager to return definitions
	//  return manager->getDefinitions();
	std::vector<WorkflowDefinition *> results;
	return results;
}

std::vector<WorkflowInstance *> WorkflowFacade::getInstances()
{
	// TODO : Add method in WorkflowManager to return instances
	// return manager->getInstances();
	std::vector<WorkflowInstance *> results;
	return results;
}

WorkflowInstance *WorkflowFacade::getInstance(const std::string &id)
{
	// TODO : Add method in WorkflowManager to return an instance
	// return manager->getInstance(id);
	return nullptr;
}
