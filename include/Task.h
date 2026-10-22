#ifndef TASK_H
#define TASK_H

class Task : WorkItem {

private:
	String name;
	String title;
	Staff responsible;

public:
	String getName();

	String getTitle();

	void printHierarchy(String indent);

	void handleRequest();

	void setNextHandler(WorkItem* next);
};

#endif
