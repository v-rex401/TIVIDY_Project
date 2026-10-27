/**
 * @file WorkflowFacade.cpp
 * @brief Implementation of WorkflowFacade: every call is delegated to the WorkflowManager.
 * @author JP
 * @date 2026-10-24
 */

#include "WorkflowFacade.h"
#include "WorkflowManager.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"

#include <algorithm>

WorkflowFacade::WorkflowFacade() : manager(new WorkflowManager()), nextInstanceNumber(1) {}

WorkflowFacade::~WorkflowFacade()
{
	// Instances only point at their definition copy's stages, so they go first.
	for (WorkflowInstance *instance : instances)
	{
		delete instance;
	}
	for (WorkflowDefinition *copy : instanceDefinitions)
	{
		delete copy;
	}
	for (WorkflowDefinition *definition : definitions)
	{
		delete definition;
	}
	delete manager;
}

void WorkflowFacade::startWorkflow(WorkflowDefinition *definition)
{
	WorkflowInstance *instance = createWorkflowInstance(definition);
	if (instance == nullptr)
	{
		return;
	}
	manager->startWorkflow(instance);
}

WorkflowInstance *WorkflowFacade::createWorkflowInstance(WorkflowDefinition *definition)
{
	if (definition == nullptr)
	{
		return nullptr;
	}

	WorkflowDefinition *copy = definition->clone();
	WorkflowInstance *instance = manager->createInstance(copy, "WF-" + std::to_string(nextInstanceNumber));
	if (instance == nullptr)
	{
		delete copy;
		return nullptr;
	}
	++nextInstanceNumber;

	for (WorkItem *stage : *copy->getStages())
	{
		instance->addWorkItem(stage);
	}

	instances.push_back(instance);
	instanceDefinitions.push_back(copy);
	return instance;
}

void WorkflowFacade::submitWorkItem(WorkflowInstance *instance, WorkItem *workitem)
{
	manager->assignWorkItem(workitem, instance);
	manager->routeWorkItem(workitem, instance);
}

void WorkflowFacade::executeWorkflow(WorkflowInstance *w)
{
	if (!owns(w))
	{
		return;
	}
	// Does nothing if the manager is already running w.
	manager->startWorkflow(w);
	w->handle();
}

void WorkflowFacade::cancelWorkflow(WorkflowInstance *instance)
{
	if (!owns(instance))
	{
		return;
	}
	manager->cancelWorkflow(instance);
}

void WorkflowFacade::addDefinition(WorkflowDefinition *definition)
{
	if (definition == nullptr ||
		std::find(definitions.begin(), definitions.end(), definition) != definitions.end())
	{
		return;
	}
	definitions.push_back(definition);
}

std::vector<WorkflowDefinition *> WorkflowFacade::getDefinitions()
{
	return definitions;
}

std::vector<WorkflowInstance *> WorkflowFacade::getInstances()
{
	return instances;
}

WorkflowInstance *WorkflowFacade::getInstance(const std::string &id)
{
	for (WorkflowInstance *instance : instances)
	{
		if (instance->getId() == id)
		{
			return instance;
		}
	}
	return nullptr;
}

bool WorkflowFacade::owns(WorkflowInstance *instance) const
{
	return instance != nullptr &&
		   std::find(instances.begin(), instances.end(), instance) != instances.end();
}
