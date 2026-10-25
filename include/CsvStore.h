#ifndef CSVSTORE_H
#define CSVSTORE_H

#include <string>
#include <vector>

/**
 * @brief Reads and writes CSV files as plain rows of fields.
 *
 * Knows about files and CSV quoting only. It has no knowledge of workflows,
 * stages, participants or any industry, so the generic engine never depends
 * on a storage format.
 */
class CsvStore {
    public:
	/**
	 * @brief Read a CSV file into rows of fields.
	 * @param path Path to the file to read.
	 * @return One vector of fields per line; empty if the file cannot be opened.
	 */
	static std::vector<std::vector<std::string> > read(const std::string& path);

	/**
	 * @brief Write rows of fields to a CSV file, overwriting it.
	 * @param path Path to the file to write.
	 * @param rows One vector of fields per line.
	 * @return True if the file was written.
	 */
	static bool write(const std::string& path, const std::vector<std::vector<std::string> >& rows);

	/**
	 * @brief Quote a single field if it contains a comma, quote or newline.
	 * @param field The raw field value.
	 * @return The field, quoted and escaped where necessary.
	 */
	static std::string escape(const std::string& field);

	/**
	 * @brief Split one CSV line into its fields, honouring quoted fields.
	 * @param line A single line of CSV, without its line terminator.
	 * @return The fields in order.
	 */
	static std::vector<std::string> parseLine(const std::string& line);
};

#endif