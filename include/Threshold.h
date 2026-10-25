#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "Stage.h"

class Threshold {
private:
    double limit;
    Stage* targetStage;

public:
    Threshold(double limit, Stage* targetStage);
    bool isTriggered(double incomingValue) const;
    Stage* getTargetStage() const;
};

#endif
