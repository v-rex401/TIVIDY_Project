/**
 * @file ClientTest.cpp
 * @brief Tests for the Client menu loop: navigation, input validation and clean exit.
 * @author JP
 * @date 2026-10-24
 */

#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "Client.h"

namespace {

/** @brief Run a Client on @p input and return everything it printed. */
std::string runWith(const std::string& input) {
	std::istringstream in(input);
	std::ostringstream out;
	Client client(nullptr, in, out);
	client.run();
	return out.str();
}

/** @brief Count how often @p needle appears in @p haystack. */
int countOf(const std::string& haystack, const std::string& needle) {
	int count = 0;
	for (std::size_t pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + 1)) {
		++count;
	}
	return count;
}

} // namespace

TEST(ClientTest, ExitsCleanlyOnEmptyInput) {
	std::string output = runWith("");
	EXPECT_EQ(countOf(output, "== TIVIDY =="), 1);
	EXPECT_NE(output.find("Goodbye."), std::string::npos);
}

TEST(ClientTest, ExitOptionStopsTheLoop) {
	std::string output = runWith("0\n1\n");
	EXPECT_EQ(countOf(output, "== TIVIDY =="), 1);
	EXPECT_NE(output.find("Goodbye."), std::string::npos);
}

TEST(ClientTest, RejectsNonNumericInputAndReprompts) {
	std::string output = runWith("abc\n0\n");
	EXPECT_EQ(countOf(output, "Invalid choice"), 1);
	EXPECT_EQ(countOf(output, "== TIVIDY =="), 2);
}

TEST(ClientTest, RejectsOutOfRangeAndNegativeNumbers) {
	std::string output = runWith("3\n-1\n99999999999\n0\n");
	EXPECT_EQ(countOf(output, "Invalid choice"), 3);
}

TEST(ClientTest, RejectsBlankLinesAndMixedInput) {
	std::string output = runWith("\n1x\n1 2\n0\n");
	EXPECT_EQ(countOf(output, "Invalid choice"), 3);
}

TEST(ClientTest, AcceptsChoiceWithSurroundingWhitespace) {
	std::string output = runWith("  0  \n");
	EXPECT_EQ(countOf(output, "Invalid choice"), 0);
	EXPECT_EQ(countOf(output, "== TIVIDY =="), 1);
}

TEST(ClientTest, BackFromInstanceMenuReturnsToMainMenu) {
	std::string output = runWith("2\n0\n0\n");
	EXPECT_EQ(countOf(output, "== Workflow instance =="), 1);
	EXPECT_EQ(countOf(output, "== TIVIDY =="), 2);
}

TEST(ClientTest, InstanceMenuStaysOpenUntilBack) {
	std::string output = runWith("2\n1\n4\n0\n0\n");
	EXPECT_EQ(countOf(output, "== Workflow instance =="), 3);
}

TEST(ClientTest, EndOfInputInsideSubMenuExitsCleanly) {
	std::string output = runWith("2\n");
	EXPECT_EQ(countOf(output, "== Workflow instance =="), 1);
	EXPECT_EQ(countOf(output, "== TIVIDY =="), 1);
	EXPECT_NE(output.find("Goodbye."), std::string::npos);
}

TEST(ClientTest, SeparateClientsDoNotShareState) {
	std::istringstream firstIn("0\n");
	std::ostringstream firstOut;
	Client first(nullptr, firstIn, firstOut);

	std::istringstream secondIn("2\n0\n0\n");
	std::ostringstream secondOut;
	Client second(nullptr, secondIn, secondOut);

	first.run();
	second.run();

	EXPECT_EQ(countOf(firstOut.str(), "== Workflow instance =="), 0);
	EXPECT_EQ(countOf(secondOut.str(), "== Workflow instance =="), 1);
}
