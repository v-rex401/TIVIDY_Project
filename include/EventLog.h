#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <vector>
#include "WorkflowEvent.h"

using namespace std;

/**
 * @brief Ordered, inspectable execution history for one workflow instance.
 *
 * Owns the WorkflowEvent objects it is given and destroys them with itself.
 */

class EventLog {

private:
	vector<WorkflowEvent*> entries;

public:

	EventLog();

	/** @brief Destroys every event held by this log. */
	~EventLog();

	/**
	 * @brief Append an event. The log takes ownership of @p e.
	 */
	void addEntry(WorkflowEvent* e);

	/**
	 * @brief All events in the order they occurred.
	 * @return Const reference, callers must not modify or delete the events.
	 */
	const vector<WorkflowEvent*>& getEntries() const;
};

#endif
