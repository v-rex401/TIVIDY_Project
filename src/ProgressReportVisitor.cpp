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

	this->report += "Stage: " + s->getName() + "\n";
}

void ProgressReportVisitor::visitTask(Task *t)
{
	if (t == NULL)
	{
		return;
	}
	std::cout << "Task: ";
	std::cout << t->getName() << std::endl;

	this->total++;
	Staff *who = t->getResponsible();
	if (who != NULL)
	{
		this->completed++;
		this->report += "  Task: " + t->getName() + ", " + who->getName() + "\n";
	}
	else
	{
		this->report += "  Task: " + t->getName() + ", nobody yet\n";
	}
}

void ProgressReportVisitor::visitWorkItem(WorkItem *w)
{
	std::cout << "WorkItem: ";
	std::cout << w->getName() << std::endl;
	w->printHierarchy("");
}

void ProgressReportVisitor::visitParticipant(Staff *p)
{
	std::cout << "Participant: ";
	std::cout << p->getName() << std::endl;
	p->printHierarchy("");

	this->report += "Participant: " + p->getName() + " (" + p->getTitle() + ")\n";
}

void ProgressReportVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	std::cout << e->getDescription() << std::endl;

	this->report += "History: " + e->getDescription() + "\n";
}

std::string ProgressReportVisitor::getReport()
{
	std::ostringstream out;
	out << this->report
	    << this->completed << " of " << this->total << " tasks have someone responsible\n";
	return out.str();
}
