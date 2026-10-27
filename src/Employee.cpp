/**
 * @file Employee.cpp
 * @brief Implementation of the Employee's details.
 * @author Jezelle Govender
 * @date 2026-10-22
 */

#include "Employee.h"
#include <iostream>

Employee::Employee(std::string name, std::string title, int rank) {
    this->name = name;
    this->title = title;
    this->rank = rank;
}

std::string Employee::getName() {
	return this->name;
}

std::string Employee::getTitle() {
	return this->title;
}

int Employee::getRank() {
	return this->rank;
}

void Employee::printHierarchy(std::string indent) {
    std::cout << indent << "Employee: " << this->name << " (Rank: " << this->rank << ")\n";
}

bool Employee::handleRequest(int rank) {
    if(this->rank >= rank)
    {
        return true;
    }

    if(getNextHandler() != nullptr)
    {
        return getNextHandler()->handleRequest(rank);
    }

    return false;
}
