#ifndef PROGRESSREPORTVISITOR_H
#define PROGRESSREPORTVISITOR_H

#include "WorkflowVisitor.h"

class ProgressReportVisitor : public WorkflowVisitor
{

private:
	int completed;      ///< Tasks visited that have someone responsible.
	int total;          ///< Tasks visited.
	std::string report; ///< Text built up as elements are visited.

public:
	/** @brief Starts with an empty report and no items counted. */
	ProgressReportVisitor();

	void visitStage(Stage *s);

	void visitTask(Task *t);

	void visitWorkItem(WorkItem *w);

	void visitParticipant(Staff *p);

	void visitHistoryEntry(WorkflowEvent *e);

	std::string getReport();
};

#endif
