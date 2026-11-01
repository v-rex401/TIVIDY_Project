/**
 * @file Employee.cpp
 * @brief Implementation of the Employee's details.
 * @author Jezelle Govender
 * @date 2026-10-22
 */

#include "Employee.h"
#include <iostream>

/**
 * @brief Constructs an Employee with a name, title and rank
 * @param name Name of the Employee
 * @param title Job title of the Employee
 * @param rank Authority rank of the Employee
 */
Employee::Employee(std::string name, std::string title, int rank) {
    this->name = name;
    this->title = title;
    this->rank = rank;
}

/**
 * @brief Gets the name of the Employee
 * @return The Employee's name
 */
std::string Employee::getName() {
	return this->name;
}

/**
 * @brief Gets the job title of the Employee
 * @return The Employee's title
 */
std::string Employee::getTitle() {
	return this->title;
}

/**
 * @brief Gets the authority rank of the Employee
 * @return The Employee's rank
 */
int Employee::getRank() {
	return this->rank;
}

/**
 * @brief Prints the Employee's details in the Staff hierarchy
 * Displays the Employee's name and rank with the specified indentation
 * @param indent String used for indentation
 */
void Employee::printHierarchy(std::string indent) {
    std::cout << indent << "Employee: " << this->name << " (Rank: " << this->rank << ")\n";
}

/**
 * @brief Attempts to handle a request based on the required Staff rank
 * Handles the request if the Employee's rank meets or exceeds the required rank
 * Otherwise, forwards the request to the next Staff member in the Chain of Responsibility
 * @param rank Minimum Staff rank required to handle the request
 * @return true if the request was handled, otherwise false
 */
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
