#include "ProgressReportVisitor.h"

void ProgressReportVisitor::visitStage(Stage* s) {
	// TODO - implement ProgressReportVisitor::visitStage
	throw "Not yet implemented";
}

void ProgressReportVisitor::visitTask(Task* Task) {
	// TODO - implement ProgressReportVisitor::visitTask
	throw "Not yet implemented";
}

void ProgressReportVisitor::visitWorkItem(WorkItem* w) {
	// TODO - implement ProgressReportVisitor::visitWorkItem
	throw "Not yet implemented";
}

void ProgressReportVisitor::visitParticipant(Staff* p) {
	// TODO - implement ProgressReportVisitor::visitParticipant
	throw "Not yet implemented";
}

void ProgressReportVisitor::visitHistoryEntry(WorkflowEvent* e) {
	// TODO - implement ProgressReportVisitor::visitHistoryEntry
	throw "Not yet implemented";
}

string ProgressReportVisitor::getReport() {
	return this->report;
}
