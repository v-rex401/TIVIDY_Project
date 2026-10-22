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
	void visitStage(Stage *s);

	void visitTask(Task *Task);

	void visitWorkItem(WorkItem *w);

	void visitParticipant(Staff *p);

	void visitHistoryEntry(WorkflowEvent *e);
};

#endif
