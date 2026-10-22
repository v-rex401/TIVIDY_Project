#ifndef STAGE_H
#define STAGE_H

class Stage : WorkItem {

private:
	string name;
	string title;
	vector<WorkItem2*> members;
	Staff* responsible;

public:
	void addMember(WorkItem2* c);

	void removeMember(WorkItem2* c);

	void handleRequest();

	void setNextHandler(WorkItem2* next);

	void accept(WorkflowVisitor* v);

	WorkflowItem* clone();

	string getName();

	string getTitle();

	void printHierarchy(string indent);
};

#endif
