/**
 * @file Team.cpp
 * @brief Implementation of the team hierarchy.
 * @author Jezelle Govender
 * @date 2026-10-22
 */

#include "Team.h"
#include "Employee.h"
#include <iostream>

std::string Team::getName() {
	return this->name;
}

std::string Team::getTitle() {
	return this->title;
}

void Team::operation() {
	//no logic specified in the UML.
}

/**
 * @brief Prints the team hierarchy.
 * @param indent The string that is used for indentation formatting.
 */
void Team::printHierarchy(std::string indent) {
	std::cout << indent << "Team: " << name << " (Rank: " << rank << ")\n"; 

	std::string memberIndent = indent + " ";
	for(size_t i = 0; i < members.size(); ++i) {
		//std::cout << memberIndent << "- [Staff Member]\n";

		//Recursing into sub-teams
		Team *t = dynamic_cast<Team*>(members[i]);
		if(t != nullptr) {
			t->printHierarchy(memberIndent);
			continue;
		}

		Employee *e = dynamic_cast<Employee*>(members[i]);
		if(e != nullptr) {
			std::cout << memberIndent << "Employee: " << e->getName() << " (Rank : " << e->getRank() << ")\n";
		}
	}
}

int Team::getRank() {
	return this->rank;
}
