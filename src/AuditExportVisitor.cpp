#include "AuditExportVisitor.h"
#include <iostream>

// TODO: Check on the indent
void AuditExportVisitor::visitStage(Stage *s)
{
	std::cout << "Stage: ";
	std::cout << s->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << s->getTitle() << std::endl;
	s->printHierarchy("Indent");
}

void AuditExportVisitor::visitTask(Task *t)
{
	std::cout << "Task: ";
	std::cout << t->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << t->getTitle() << std::endl;
	t->printHierarchy("Indent");
}

void AuditExportVisitor::visitWorkItem(WorkItem *w)
{
	std::cout << "WorkItem: ";
	std::cout << w->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << w->getTitle() << std::endl;
	w->printHierarchy("Indent");
}

void AuditExportVisitor::visitParticipant(Staff *p)
{
	std::cout << "Participant: ";
	std::cout << p->getName() << std::endl;
	std::cout << "Title: ";
	std::cout << p->getTitle() << std::endl;
	p->printHierarchy("Indent");
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
