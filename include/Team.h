#ifndef TEAM_H
#define TEAM_H
#include <vector>
#include <string>
#include "Staff.h"

class Team : public Staff {

private:
	std::string name;
	std::string title;
	int rank = 0;
	std::vector<Staff*> members;

public:
	/**
	 * @brief This constructs a Team.
	 * It initializes an empty team ready to have members added to it.
	 */
	Team(std::string name, std::string title, int rank);
	~Team() override;
	std::string getName() override;
	std::string getTitle() override;
	void operation();
	void printHierarchy(std::string indent) override;
	int getRank() override;

	/**
     * @brief Adds a staff member to the team
     * @param member Pointer to the staff member
     */
    void addMember(Staff* member);

	/**
     * @brief Removes a staff member from the team
     * @param member Pointer to the staff member
     */
    void removeMember(Staff* member);

	/**
     * @brief Attempts to handle a request based on the required rank
     * @param rank Minimum rank required to handle the request
     * @return true if handled, otherwise false
     */
    bool handleRequest(int rank) override;
};

#endif
