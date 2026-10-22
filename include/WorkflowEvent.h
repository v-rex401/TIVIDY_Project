#ifndef WORKFLOWEVENT_H
#define WORKFLOWEVENT_H

class WorkflowEvent {

private:
	string type;
	string description;
	string timestamp;

public:
	string getType();

	string getDescription();

	string getTimestamp();
};

#endif
