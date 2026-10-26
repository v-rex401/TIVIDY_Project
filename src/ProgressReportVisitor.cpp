#include "ProgressReportVisitor.h"
#include <iostream>
#include <string>
#include <sstream>

ProgressReportVisitor::ProgressReportVisitor()
	: completed(0), total(0), report("")
{
}

void ProgressReportVisitor::visitStage(Stage *s)
{
	if (s == NULL)
	{
		return;
	}
	std::cout << "Stage: ";
	std::cout << s->getName() << std::endl;

	this->total++;
	this->report += "Stage: " + s->getName() + "\n";
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
	w->printHierarchy("Indent");
}

void ProgressReportVisitor::visitParticipant(Staff *p)
{
	std::cout << "Participant: ";
	std::cout << p->getName() << std::endl;
	p->printHierarchy("Indent");
}

void ProgressReportVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	std::cout << e->getDescription() << std::endl;
}

std::string ProgressReportVisitor::getReport()
{
	std::ostringstream out;
	out << this->report
	    << this->completed << " of " << this->total << " work items reported complete\n";
	return out.str();
}
