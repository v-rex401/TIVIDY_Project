#ifndef TEAM_H
#define TEAM_H
#include <vector>
#include <string>
#include "Staff.h"

class Team : public Staff {

private:
	std::string name;
	std::string title;
	int rank;
	std::vector<Staff*> members;

public:
	std::string getName();
	std::string getTitle();
	void operation();
	void printHierarchy(std::string indent);
	int getRank();
};

#endif
