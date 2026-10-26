#ifndef WORKFLOWVISITOR_H
#define WORKFLOWVISITOR_H

#include "Stage.h"
#include "Task.h"
#include "WorkItem.h"
#include "WorkItem.h"
#include "Staff.h"
#include "WorkflowEvent.h"

class WorkflowVisitor
{

public:
	virtual void visitStage(Stage *s) = 0;

	virtual void visitTask(Task *Task) = 0;

	virtual void visitWorkItem(WorkItem *w) = 0;

	virtual void visitParticipant(Staff *p) = 0;

	virtual void visitHistoryEntry(WorkflowEvent *e) = 0;
};

#endif
