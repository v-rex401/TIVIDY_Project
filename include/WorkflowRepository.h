#ifndef WORKFLOWREPOSITORY_H
#define WORKFLOWREPOSITORY_H

#include <string>

class EventLog;

/**
 * @brief Persists and restores workflow data through a file store.
 *
 * Knows the shape of workflow data (an event has a type, a description and a
 * timestamp) but nothing about any industry: industry detail arrives as the
 * contents of the file, not as code. Delegates all file and CSV handling to
 * CsvStore, so EventLog never learns what a file is.
 */
class WorkflowRepository {
    public:
	/**
	 * @brief Write a log's events to a CSV file as type,description,timestamp.
	 * @param log The history to export.
	 * @param path File to write, overwritten if it exists.
	 * @return True if the file was written.
	 */
	static bool saveHistory(const EventLog& log, const std::string& path);
    /**
	 * @brief Append events read from a CSV file into a log.
	 * @param log The log to append to; it takes ownership of the new events.
	 * @param path File to read.
	 * @return Number of events appended; 0 if the file is missing or empty.
	 *
	 * Rows with fewer than three fields are skipped and a leading header row is
	 * ignored, so a hand-edited file cannot corrupt the history.
	 */
    static int loadHistory(EventLog& log, const std::string& path);
};

#endif