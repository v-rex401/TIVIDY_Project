#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "Stage.h"

/**
 * @brief Associates a numeric threshold with a destination stage
 * 
 * A threshold is triggered when an incoming value is greater than or equal
 * to its configured limit
 */
class Threshold {
private:
    double limit;
    Stage* targetStage;

public:
    /**
     * @brief Constructor for Threshold
     * @param limit Minimum value that triggers this threshold
     * @param targetStage Stage selected when the threshold is triggered
     */
    Threshold(double limit, Stage* targetStage);

    /**
     * @brief Check whether a value reaches this threshold
     * @param incomingValue Value to compare with the limit
     * @return True when incomingValue is greater than or equal to the limit
     */
    bool isTriggered(double incomingValue) const;

    /**
     * @brief Get the stage associated with this threshold
     */
    Stage* getTargetStage() const;

    /**
     * @brief Get the numeric trigger limit
     */
    double getLimit() const;
};

#endif
