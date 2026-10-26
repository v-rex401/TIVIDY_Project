/**
 * @file WorkflowRepository.cpp
 * @brief Implementation of workflow persistence through CsvStore.
 * @author Anke de Frey
 * @date 2026-10-26
 */

#include "WorkflowRepository.h"
#include "CsvStore.h"
#include "EventLog.h"
#include "WorkflowEvent.h"

bool WorkflowRepository::saveHistory(const EventLog& log, const std::string& path) {
	std::vector<std::vector<std::string> > rows;

	std::vector<std::string> header;
	header.push_back("type");
	header.push_back("description");
	header.push_back("timestamp");
	rows.push_back(header);

	const std::vector<WorkflowEvent*>& entries = log.getEntries();
	for (WorkflowEvent* e : entries) {
		if (e == NULL) {
			continue;
		}
		std::vector<std::string> row;
		row.push_back(e->getType());
		row.push_back(e->getDescription());
		row.push_back(e->getTimestamp());
		rows.push_back(row);
	}

	return CsvStore::write(path, rows);
}

int WorkflowRepository::loadHistory(EventLog& log, const std::string& path) {
    std::vector<std::vector<std::string> > rows = CsvStore::read(path);
	int added = 0;
	bool firstRow = true;

	for (const std::vector<std::string>& row : rows) {
		if (firstRow) {
			firstRow = false;
			if (!row.empty() && row[0] == "type") {
				continue;   // header line, not an event
			}
		}
		if (row.size() < 3) {
			continue;
		}
		log.addEntry(new WorkflowEvent(row[0], row[1], row[2]));
		added++;
	}

	return added;

}