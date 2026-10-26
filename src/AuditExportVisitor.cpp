#include "AuditExportVisitor.h"
#include <iostream>

void AuditExportVisitor::visitStage(Stage *s)
{
	std::cout << "Stage: ";
	std::cout << s->getName() << std::endl;
}

void AuditExportVisitor::visitTask(Task *t)
{
	std::cout << "Task: ";
	std::cout << t->getName() << std::endl;
}

void AuditExportVisitor::visitWorkItem(WorkItem *w)
{
	std::cout << "WorkItem: ";
	std::cout << w->getName() << std::endl;
	// TODO: Print Hierarchy
}

void AuditExportVisitor::visitParticipant(Staff * /* p */)
{
	std::cout << "Staff: ";
	// TODO: Need to put functions in the Staff class
}

void AuditExportVisitor::visitHistoryEntry(WorkflowEvent *e)
{
	std::cout << e->getDescription() << std::endl;
}
std::string AuditExportVisitor::getAuditTrail()
{
	std::string result = "Audit Trail\n";
	result += trail;

	return result;
}
