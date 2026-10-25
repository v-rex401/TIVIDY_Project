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
	// TODO: Print Hierarchy
}

void ProgressReportVisitor::visitParticipant(Staff * /* p */)
{
	std::cout << "Staff: ";
	// TODO: Need to put functions in the Staff class
}

void ProgressReportVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	std::cout << e->getDescription() << std::endl;
}

std::string ProgressReportVisitor::getReport()
{
	return this->report;
}
