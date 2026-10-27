/**
 * @file RuleEngineTests.cpp
 * @brief Behaviour tests for the Assignment helper class in RuleBasedAssignment.
 * @author Azolile Mbanga
 * @date 2026-10-26
 */

#include <gtest/gtest.h>
#include "RuleEngine.h"

class TestWorkItem : public WorkItem {
public:
    std::string name = "Review request";
    std::string title = "Review";
    std::string severity = "High";
    std::string requiredPosition = "Senior Reviewer";

    std::string getName() override { return name; }
    std::string getTitle() override { return title; }
    std::string getSeverity() override { return severity; }
    std::string getRequiredPosition() override { return requiredPosition; }
    void setRequiredPosition(const std::string& position) override { requiredPosition = position; }
    void printHierarchy(std::string) override {}
    void handleRequest() override {}
    WorkItem* clone() override { return new TestWorkItem(*this); }
};

//Check that the field name == 'Review Request'
TEST(RuleEngineTests, MatchesNameField) {
    RuleEngine engine;
    TestWorkItem item;
    AssignmentRule rule;
    rule.conditionField = "name";
    rule.expectedValue = "Review request";
    EXPECT_TRUE(engine.evaluate(rule, &item, nullptr));
}

//Check that the field title == 'Review'
TEST(RuleEngineTests, MatchesTitleField) {
    RuleEngine engine;
    TestWorkItem item;
    AssignmentRule rule;
    rule.conditionField = "title";
    rule.expectedValue = "Review";
    EXPECT_TRUE(engine.evaluate(rule, &item, nullptr));
}

//Check that the field requiredPosition == 'Senior Reviewer'
TEST(RuleEngineTests, MatchesRequiredPositionField) {
    RuleEngine engine;
    TestWorkItem item;
    AssignmentRule rule;
    rule.conditionField = "requiredPosition";
    rule.expectedValue = "Senior Reviewer";
    EXPECT_TRUE(engine.evaluate(rule, &item, nullptr));
}

//Check that the field title != 'Review' so it should fail (return false)
TEST(RuleEngineTests, RejectWrongValueUnknwonFieldOrNullItem) {
    RuleEngine engine;
    TestWorkItem item;
    AssignmentRule rule;
    rule.conditionField = "title";
    rule.expectedValue = "Administrator";
    EXPECT_FALSE(engine.evaluate(rule, &item, nullptr));
}