#include <gtest/gtest.h>
#include "Threshold.h"
#include "Stage.h"

TEST(ThresholdTests, ValueBelowLimitDoesNotTrigger) {
    Stage destination;
    Threshold threshold(10.0, &destination);
    EXPECT_FALSE(threshold.isTriggered(9.99));
}

TEST(ThresholdTests, ValueEqualToLimitTriggers) {
    Stage destination;
    Threshold threshold(10.0, &destination);
    EXPECT_TRUE(threshold.isTriggered(10.0));
}

TEST(ThresholdTests, ValueAboveLimitTriggers) {
    Stage destination;
    Threshold threshold(10.0, &destination);
    EXPECT_TRUE(threshold.isTriggered(10.01));
}

TEST(ThresholdTests, ReturnLimitandDestination) {
    Stage destination;
    Threshold threshold(42.5, &destination);
    EXPECT_DOUBLE_EQ(threshold.getLimit(), 42.5);
    EXPECT_EQ(threshold.getTargetStage(), &destination);
}
