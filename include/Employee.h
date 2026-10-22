#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <string>
#include "Staff.h"

class Employee : public Staff {

private:
	std::string name;
	std::string title;
	int rank;

public:
	std::string getName();
	std::string getTitle();
	int getRank();
};

#endif
