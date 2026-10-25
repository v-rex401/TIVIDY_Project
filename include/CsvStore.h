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
};

#endif