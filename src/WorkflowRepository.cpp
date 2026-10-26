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
	for (size_t i = 0; i < entries.size(); i++) {
		if (entries[i] == NULL) {
			continue;
		}
		std::vector<std::string> row;
		row.push_back(entries[i]->getType());
		row.push_back(entries[i]->getDescription());
		row.push_back(entries[i]->getTimestamp());
		rows.push_back(row);
	}

	return CsvStore::write(path, rows);
}

int WorkflowRepository::loadHistory(EventLog& log, const std::string& path) {

}