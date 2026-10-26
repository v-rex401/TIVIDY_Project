/**
 * @file PersistenceTest.cpp
 * @brief Behaviour tests for CSV storage and workflow persistence.
 * @author Anke de Frey
 * @date 2026-10-25
 */

#include <gtest/gtest.h>
#include <cstdio>
#include "CsvStore.h"
#include "EventLog.h"
#include "WorkflowEvent.h"
#include "WorkflowRepository.h"

TEST(CsvStoreTest, LeavesPlainFieldsAlone) {
    EXPECT_EQ("plain", CsvStore::escape("plain"));
}

TEST(CsvStoreTest, QuotesFieldsContainingDelimiters) {
    EXPECT_EQ("\"has,comma\"", CsvStore::escape("has,comma"));
    EXPECT_EQ("\"say \"\"hi\"\"\"", CsvStore::escape("say \"hi\""));
}

TEST(CsvStoreTest, ParsesQuotedFields) {
    std::vector<std::string> f = CsvStore::parseLine("a,\"b,c\",\"say \"\"hi\"\"\"");
    ASSERT_EQ(3u, f.size());
    EXPECT_EQ("a", f[0]);
    EXPECT_EQ("b,c", f[1]);
    EXPECT_EQ("say \"hi\"", f[2]);
}

/** Invalid input: a missing file must read as empty, not crash. */
TEST(CsvStoreTest, MissingFileReadsAsEmpty) {
    EXPECT_TRUE(CsvStore::read("no_such_file_12345.csv").empty());
}

/** History must survive a trip to disk and back unchanged. */
TEST(WorkflowRepositoryTest, HistoryRoundTripsThroughFile) {
    const std::string path = "test_history_roundtrip.csv";

    {
        EventLog log;
        log.addEntry(new WorkflowEvent("STATE_CHANGE", "Pending -> Active, approved", "t1"));
        log.addEntry(new WorkflowEvent("ACTION", "Said \"yes\"", "t2"));
        ASSERT_TRUE(WorkflowRepository::saveHistory(log, path));
    }

    EventLog restored;
    EXPECT_EQ(2, WorkflowRepository::loadHistory(restored, path));
    ASSERT_EQ(2u, restored.getEntries().size());
    EXPECT_EQ("STATE_CHANGE", restored.getEntries()[0]->getType());
    EXPECT_EQ("Pending -> Active, approved", restored.getEntries()[0]->getDescription());
    EXPECT_EQ("Said \"yes\"", restored.getEntries()[1]->getDescription());
    EXPECT_EQ("t2", restored.getEntries()[1]->getTimestamp());

    std::remove(path.c_str());
}

/** Invalid operation: a missing file must not corrupt the log. */
TEST(WorkflowRepositoryTest, LoadingMissingFileAddsNothing) {
    EventLog log;
    EXPECT_EQ(0, WorkflowRepository::loadHistory(log, "no_such_file_12345.csv"));
    EXPECT_TRUE(log.getEntries().empty());
}

/** An empty log still writes a readable file with just its header. */
TEST(WorkflowRepositoryTest, EmptyLogWritesHeaderOnly) {
    const std::string path = "test_empty_history.csv";
    EventLog log;
    ASSERT_TRUE(WorkflowRepository::saveHistory(log, path));

    EventLog restored;
    EXPECT_EQ(0, WorkflowRepository::loadHistory(restored, path));
    EXPECT_TRUE(restored.getEntries().empty());

    std::remove(path.c_str());
}
