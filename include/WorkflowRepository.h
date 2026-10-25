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
};

#endif