/**
 * @file TextDemoTest.cpp
 * @brief Tests that the scripted run covers every part of the demo and finishes.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include <sstream>

#include "Scenarios.h"
#include "TextDemo.h"

TEST(TextDemoTest, EveryScenarioRunsToTheEnd)
{
    for (const ScenarioSetup &setup : Scenarios::all())
    {
        std::ostringstream out;
        EXPECT_EQ(TextDemo::run(setup, out), 0) << setup.name;
        EXPECT_NE(out.str().find("Reports (Visitor, Observer)"), std::string::npos) << setup.name;
    }
}

TEST(TextDemoTest, StoryShowsTheKeyEvents)
{
    std::ostringstream out;
    TextDemo::run(Scenarios::hospital(), out);
    std::string story = out.str();

    EXPECT_NE(story.find("Approve: Pending → Active"), std::string::npos);
    EXPECT_NE(story.find("Undo Approve"), std::string::npos);
    EXPECT_NE(story.find("nobody can approve it"), std::string::npos);
    EXPECT_NE(story.find("Emergency Care"), std::string::npos);
}
