#ifndef HANDLER_H
#define HANDLER_H

class Handler : WorkItem {

private:
	vector<WorkItem> members;
	String name;
	String title;
	Handler* successor;
	Staff* responsible;

public:
	String getName();

	String getTitle();

	void printHierarchy(String indent);

	void addMember(int Component);

	void removeMember(int Component);

	void setNextHandler(Handler* next);

	void handleRequest();

	void setNextHandler2(WorkItem* next);
};

#endif
