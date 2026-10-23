#ifndef AUDITEXPORTVISITOR_H
#define AUDITEXPORTVISITOR_H

#include "WorkflowVisitor.h"

class AuditExportVisitor : public WorkflowVisitor
{

public:
	string trail;

	void visitStage(Stage *s);

	void visitTask(Task *t);

	void visitWorkItem(WorkItem *w);

	void visitParticipant(Staff *p);

	void visitHistoryEntry(WorkflowEvent *e);

	string getAuditTrail();
};

#endif
