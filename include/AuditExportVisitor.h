#ifndef AUDITEXPORTVISITOR_H
#define AUDITEXPORTVISITOR_H

class AuditExportVisitor : WorkflowVisitor {

public:
	string trail;

	void visitStage(Stage* s);

	void visitTask(Task* Task);

	void visitWorkItem(WorkItem* w);

	void visitParticipant(Staff* p);

	void visitHistoryEntry(WorkflowEvent* e);

	string getAuditTrail();
};

#endif
