#ifndef ASSIGNMENTRULE_H
#define ASSIGNMENTRULE_H

#include <string>

/**
 * @brief Data-driven condition mapping a work-item field to a staff title.
 * 
 * @param conditionField Field to inspect (name / title / requiredPosition)
 * @param expectedValue Value that must match for the rule to apply
 * @param targetTitle Staff title selected when the condition matches
 */
class AssignmentRule {

public:
    std::string conditionField;
    std::string expectedValue;
    std::string targetTitle;
};

#endif
