#ifndef WORKFLOWVISITOR_H
#define WORKFLOWVISITOR_H

class WorkflowVisitor {


public:
	void visitStage(Stage* s);

	void visitTask(Task* Task);

	void visitWorkItem(WorkItem* w);

	void visitParticipant(Staff* p);

	void visitHistoryEntry(WorkflowEvent* e);
};

#endif
