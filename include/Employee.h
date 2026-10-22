#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee : Staff {

private:
	String name;
	String title;
	int rank;

public:
	String getName();

	String getTitle();

	int getRank();
};

#endif
