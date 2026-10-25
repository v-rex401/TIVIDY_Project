#include "WorkflowDefinition.h"

void WorkflowDefinition::accept(WorkflowVisitor * /* wv */)
{
	// TODO - implement WorkflowDefinition::accept
}

WorkflowDefinition *WorkflowDefinition::clone()
{
	// TODO - implement WorkflowDefinition::clone
	return nullptr;
}

void WorkflowDefinition::addStage(WorkItem * /* s */)
{
	// TODO - implement WorkflowDefinition::addStage
}

std::vector<WorkItem *> *WorkflowDefinition::getStages()
{
	// TODO - implement WorkflowDefinition::getStages
	return nullptr;
}

void WorkflowDefinition::setBuilder(WorkflowBuilder * /* b */)
{
	// TODO - implement WorkflowDefinition::setBuilder
}
