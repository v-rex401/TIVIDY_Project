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
	virtual void visitStage(Stage *s);

	virtual void visitTask(Task *Task);

	virtual void visitWorkItem(WorkItem *w);

	virtual void visitParticipant(Staff *p);

	virtual void visitHistoryEntry(WorkflowEvent *e);
};

#endif
