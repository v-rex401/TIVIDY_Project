#ifndef EVENTLOG_H
#define EVENTLOG_H

class EventLog {

private:
	vector<WorkflowEvent*> entries;

public:
	void addEntry(WorkflowEvent* e);

	vector<WorklflowEvent*> getEntries();
};

#endif
