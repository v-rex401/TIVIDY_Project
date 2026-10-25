/**
 * @file CsvStore.cpp
 * @brief Implementation of CSV file reading and writing.
 * @author Anke de Frey
 * @date 2026-10-25
 */

#include "CsvStore.h"
#include <fstream>

std::string CsvStore::escape(const std::string& field) {
	bool needsQuotes = false;
	for (size_t i = 0; i < field.size(); i++) {
		if (field[i] == ',' || field[i] == '"' || field[i] == '\n' || field[i] == '\r') {
			needsQuotes = true;
			break;
		}
	}
	if (!needsQuotes) {
		return field;
	}

	std::string out = "\"";
	for (size_t i = 0; i < field.size(); i++) {
		if (field[i] == '"') {
			out += "\"\"";
		} else {
			out += field[i];
		}
	}
	out += "\"";
	return out;
}

std::vector<std::string> CsvStore::parseLine(const std::string& line) {
    std::vector<std::string> fields;
	std::string current = "";
	bool inQuotes = false;

	for (size_t i = 0; i < line.size(); i++) {
		char c = line[i];
		if (inQuotes) {
			if (c == '"') {
				if (i + 1 < line.size() && line[i + 1] == '"') {
					current += '"';
					i++;
				} else {
					inQuotes = false;
				}
			} else {
				current += c;
			}
		} else {
			if (c == '"') {
				inQuotes = true;
			} else if (c == ',') {
				fields.push_back(current);
				current = "";
			} else if (c != '\r') {
				current += c;
			}
		}
	}
	fields.push_back(current);
	return fields;
}

std::vector<std::vector<std::string> > CsvStore::read(const std::string& path) {
}

bool CsvStore::write(const std::string& path, const std::vector<std::vector<std::string> >& rows) {
}
