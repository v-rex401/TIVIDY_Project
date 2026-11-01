#include "Threshold.h"

Threshold::Threshold(double limit, Stage* targetStage) {
    this->limit = limit;
    this->targetStage = targetStage;
}

bool Threshold::isTriggered(double incomingValue) const {
    return incomingValue >= limit;
}
    
Stage* Threshold::getTargetStage() const {
    return this->targetStage;
}

double Threshold::getLimit() const {
    return this->limit;
}