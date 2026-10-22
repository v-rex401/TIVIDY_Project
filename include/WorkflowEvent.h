#ifndef WORKFLOWEVENT_H
#define WORKFLOWEVENT_H

#include <string>
using namespace std;

/**
 * @brief A single immutable record of something that happened in the workflow instance.
 * 
 * Created by WorkflowInstance on a state change or action and handed to the observers
 * AuditLogObserver appends it to the EventLog
 */

class WorkflowEvent {

private:
	string type;
	string description;
	string timestamp;

public:
	/**
	 * @brief Construct a fully populated event
	 * @param type Short event kind eg "STATE_CHANGE", "ACTION", "ESCALATION"
	 * @param description Human-readable detail shown in the history view
	 * @param timestamp When the event occurred.
	 */
	WorkflowEvent(const std::string& type, const std::string& description, const std::string& timestamp);
	string getType() const;

	string getDescription() const;

	string getTimestamp() const;
};

#endif
