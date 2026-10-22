#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <vector>

#include "WorkItem.h"

class Stage : public WorkItem {

private:
	std::string name;
	std::string title;
	std::vector<WorkItem*> members;
	Staff* responsible;

public:
	void addMember(WorkItem* c);

	void removeMember(WorkItem* c);

	void handleRequest();

	void setNextHandler(WorkItem* next);

	void accept(WorkflowVisitor* v);

	WorkItem* clone();

	std::string getName();

	std::string getTitle();

	void printHierarchy(std::string indent);
};

#endif
