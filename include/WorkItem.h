#ifndef WORKITEM_H
#define WORKITEM_H

class WorkItem {

private:
	WorkItem* successor;

public:
	virtual String getName() = 0;

	virtual String getTitle() = 0;

	virtual void printHierarchy(String indent) = 0;

	virtual void handleRequest() = 0;

	void setNextHandler(WorkItem* next);
};

#endif
