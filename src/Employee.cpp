/**
 * @file Employee.cpp
 * @brief Implementation of the Employee's details.
 * @author Jezelle Govender
 * @date 2026-10-22
 */

#include "Employee.h"

std::string Employee::getName() {
	return this->name;
}

std::string Employee::getTitle() {
	return this->title;
}

int Employee::getRank() {
	return this->rank;
}
