#include "AuditExportVisitor.h"
#include <iostream>

void AuditExportVisitor::visitStage(Stage *s)
{
	std::cout << "Stage: ";
	std::cout << s->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << s->getTitle() << std::endl;
	s->printHierarchy(0);
}

void AuditExportVisitor::visitTask(Task *t)
{
	std::cout << "Task: ";
	std::cout << t->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << t->getTitle() << std::endl;
	t->printHierarchy(0);
}

void AuditExportVisitor::visitWorkItem(WorkItem *w)
{
	std::cout << "WorkItem: ";
	std::cout << w->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << w->getTitle() << std::endl;
	w->printHierarchy(0);
}

void AuditExportVisitor::visitParticipant(Staff *p)
{
	std::cout << "Participant: ";
	std::cout << p->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << p->getTitle() << std::endl;
	p->printHierarchy(0);
}

void AuditExportVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	std::cout << e->getTimestamp() << std::endl;
	std::cout << e->getType() << std::endl;
	std::cout << e->getDescription() << std::endl;
}
std::string AuditExportVisitor::getAuditTrail()
{
	std::string result = "Audit Trail\n";
	// TODO: Determine what is being visited
	return "End of Audit Trail";
}
