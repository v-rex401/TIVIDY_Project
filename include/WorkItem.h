#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>

class WorkItem {

private:
	WorkItem* successor;

public:
	virtual std::string getName() = 0;

	virtual std::string getTitle() = 0;

	virtual void printHierarchy(std::string indent) = 0;

	virtual void handleRequest() = 0;

	void setNextHandler(WorkItem* next);

	virtual ~WorkItem() = default;
};

#endif
