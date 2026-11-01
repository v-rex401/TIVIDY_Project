#include "AuditExportVisitor.h"
#include <iostream>

// TODO: Check on the indent
void AuditExportVisitor::visitStage(Stage *s)
{
	std::cout << "Stage: ";
	std::cout << s->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << s->getTitle() << std::endl;
	s->printHierarchy("");

	this->trail += "Stage: " + s->getName() + "\n";
}

void AuditExportVisitor::visitTask(Task *t)
{
	std::cout << "Task: ";
	std::cout << t->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << t->getTitle() << std::endl;
	t->printHierarchy("");

	this->trail += "Task: " + t->getName() + "\n";
}

void AuditExportVisitor::visitWorkItem(WorkItem *w)
{
	std::cout << "WorkItem: ";
	std::cout << w->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << w->getTitle() << std::endl;
	w->printHierarchy("");

	this->trail += "WorkItem: " + w->getName() + "\n";
}

void AuditExportVisitor::visitParticipant(Staff *p)
{
	std::cout << "Participant: ";
	std::cout << p->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << p->getTitle() << std::endl;
	p->printHierarchy("");

	this->trail += "Participant: " + p->getName() + ", " + p->getTitle() + "\n";
}

void AuditExportVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	std::cout << e->getTimestamp() << std::endl;
	std::cout << e->getType() << std::endl;
	std::cout << e->getDescription() << std::endl;

	this->trail += e->getTimestamp() + "  " + e->getType() + "  " + e->getDescription() + "\n";
}
std::string AuditExportVisitor::getAuditTrail()
{
	return "Audit Trail\n" + this->trail + "End of Audit Trail\n";
}
