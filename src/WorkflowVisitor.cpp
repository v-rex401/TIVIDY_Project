#include "WorkflowVisitor.h"

void WorkflowVisitor::visitStage(Stage *s)
{
	return;
}

void WorkflowVisitor::visitTask(Task *Task)
{
	// TODO - implement WorkflowVisitor::visitTask
	throw "Not yet implemented";
}

void WorkflowVisitor::visitWorkItem(WorkItem *w)
{
	// TODO - implement WorkflowVisitor::visitWorkItem
	throw "Not yet implemented";
}

void WorkflowVisitor::visitParticipant(Staff *p)
{
	// TODO - implement WorkflowVisitor::visitParticipant
	throw "Not yet implemented";
}

void WorkflowVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	// TODO - implement WorkflowVisitor::visitHistoryEntry
	throw "Not yet implemented";
}
