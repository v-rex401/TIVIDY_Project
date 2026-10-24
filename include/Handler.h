#ifndef HANDLER_H
#define HANDLER_H

#include "WorkItem.h"
#include "Staff.h"
#include <vector>

class Handler : public WorkItem
{

private:
	std::vector<WorkItem *> members;
	std::string name;
	std::string title;
	Handler *successor;
	Staff *responsible;

public:
	std::string getName();

	std::string getTitle();

	void printHierarchy(std::string indent);

	void addMember(int Component);

	void removeMember(int Component);

	void setNextHandler(Handler *next);

	void handleRequest();

	void setNextHandler(WorkItem *next);
};

#endif
