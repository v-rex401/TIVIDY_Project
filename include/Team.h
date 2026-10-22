#ifndef TEAM_H
#define TEAM_H

class Team : Staff {

private:
	String name;
	String title;
	int rank;
	vector<Staff*> members;

public:
	String getName();

	String getTitle();

	void operation();

	void printHierarchy(String indent);

	int getRank();
};

#endif
