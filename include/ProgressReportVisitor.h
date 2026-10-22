#ifndef PROGRESSREPORTVISITOR_H
#define PROGRESSREPORTVISITOR_H

class ProgressReportVisitor : WorkflowVisitor {

private:
	int completed;
	int total;
	string report;

public:
	void visitStage(Stage* s);

	void visitTask(Task* Task);

	void visitWorkItem(WorkItem* w);

	void visitParticipant(Staff* p);

	void visitHistoryEntry(WorkflowEvent* e);

	string getReport();
};

#endif
