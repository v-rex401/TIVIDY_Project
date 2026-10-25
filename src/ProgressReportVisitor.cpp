#include "ProgressReportVisitor.h"
#include <iostream>
#include <string>

void ProgressReportVisitor::visitStage(Stage *s)
{
	std::cout << "Stage: ";
	std::cout << s->getName() << std::endl;
}

void ProgressReportVisitor::visitTask(Task *t)
{
	std::cout << "Task: ";
	std::cout << t->getName() << std::endl;
}

void ProgressReportVisitor::visitWorkItem(WorkItem *w)
{
	std::cout << "WorkItem: ";
	std::cout << w->getName() << std::endl;
	w->printHierarchy(0);
}

void ProgressReportVisitor::visitParticipant(Staff *p)
{
	std::cout << "Participant: ";
	std::cout << p->getName() << std::endl;
	p->printHierarchy(0);
}

void ProgressReportVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	std::cout << e->getDescription() << std::endl;
}

std::string ProgressReportVisitor::getReport()
{
	return this->report;
}
