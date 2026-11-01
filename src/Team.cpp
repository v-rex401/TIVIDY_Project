/**
 * @file Team.cpp
 * @brief Implementation of the team hierarchy.
 * @author Jezelle Govender
 * @date 2026-10-22
 */

#include "Team.h"
#include "Employee.h"
#include <iostream>
#include <algorithm>

/**
 * @brief Constructs a Team with a name, title and rank
 * @param name Name of the Team
 * @param title Title of the Team
 * @param rank Authority rank assigned to the Team
 */
Team::Team(std::string name, std::string title, int rank) {
    this->name = name;
    this->title = title;
    this->rank = rank;
}

/**
 * @brief Gets the name of the Team
 * @return The Team's name
 */
std::string Team::getName() {
	return this->name;
}

/**
 * @brief Gets the title of the Team
 * @return The Team's title
 */
std::string Team::getTitle() {
	return this->title;
}

/**
 * @brief Performs the Team's operation
 * Currently has no implementation, as no logic was specified in the UML.
 */
void Team::operation() {
	//no logic specified in the UML.
}

/**
 * @brief Prints the team hierarchy.
 * @param indent The string that is used for indentation formatting.
 */
void Team::printHierarchy(std::string indent) {
	std::cout << indent << "Team: " << this->name << " (Rank: " << this->rank << ")\n";

    for(Staff* member : this->members)
    {
        if(member != nullptr)
        {
            member->printHierarchy(indent + "  ");
        }
    }
}

/**
 * @brief Gets the authority rank of the Team
 * @return The Team's rank
 */
int Team::getRank() {
	return this->rank;
}

/**
 * @brief Adds a staff member to the team
 * @param member Pointer to the staff member
 */
void Team::addMember(Staff* member)
{
    if(member == nullptr)
    {
        return;
    }

    //prevents same pointer from being inserted more than once
    //this helps prevent a potential double deletion
    if(std::find(this->members.begin(), this->members.end(), member)
       != this->members.end())
    {
        return;
    }

    this->members.push_back(member);
}

/**
 * @brief Removes and deletes a staff member from the team
 * @param member Pointer to the staff member
 */
void Team::removeMember(Staff* member)
{
    for(auto it = this->members.begin(); it != this->members.end(); ++it)
    {
        if(*it == member)
        {
            delete *it;
            this->members.erase(it);
            return;
        }
    }
}

/**
 * @brief The team's direct members
 * @return The members, in the order they were added
 */
const std::vector<Staff*>& Team::getMembers() const
{
    return this->members;
}

/**
 * @brief Attempts to handle a request based on the required rank
 * @param rank Minimum rank required to handle the request
 * @return true if handled, otherwise false
 */
bool Team::handleRequest(int rank)
{
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

/**
 * @brief Destroys the Team and its owned members
 */
Team::~Team()
{
    for(Staff* member : this->members)
    {
        delete member;
    }
}
