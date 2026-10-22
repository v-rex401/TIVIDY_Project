#ifndef TASK_H
#define TASK_H

#include <string>

#include "WorkItem.h"
#include "Staff.h"

class Task : public WorkItem {

private:
	std::string name;
	std::string title;
	Staff responsible;

public:
	std::string getName();

	std::string getTitle();

	void printHierarchy(std::string indent);

	void handleRequest();

	void setNextHandler(WorkItem* next);
};

#endif
