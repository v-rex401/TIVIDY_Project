/**
 * @file CoutCaptureTest.cpp
 * @brief Tests that CoutCapture keeps engine output off the real std::cout.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "CoutCapture.h"

TEST(CoutCaptureTest, KeepsWhatIsPrinted)
{
    CoutCapture capture;
    std::cout << "[State] Pending -> Active" << std::endl;
    EXPECT_EQ(capture.text(), "[State] Pending -> Active\n");
}

TEST(CoutCaptureTest, SplitsIntoNonEmptyLines)
{
    CoutCapture capture;
    std::cout << "one\n\ntwo\r\nthree";
    std::vector<std::string> lines = capture.lines();
    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "one");
    EXPECT_EQ(lines[1], "two");
    EXPECT_EQ(lines[2], "three");
}

TEST(CoutCaptureTest, GivesTheOldBufferBackWhenItEnds)
{
    std::ostringstream outer;
    std::streambuf *original = std::cout.rdbuf(outer.rdbuf());
    {
        CoutCapture capture;
        std::cout << "hidden";
    }
    std::cout << "visible";
    std::cout.rdbuf(original);

    EXPECT_EQ(outer.str(), "visible");
}

TEST(CoutCaptureTest, NestedCapturesEachKeepTheirOwnText)
{
    CoutCapture outer;
    std::cout << "a";
    {
        CoutCapture inner;
        std::cout << "b";
        EXPECT_EQ(inner.text(), "b");
    }
    std::cout << "c";
    EXPECT_EQ(outer.text(), "ac");
}
