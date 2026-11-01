#ifndef TEAM_H
#define TEAM_H
#include <vector>
#include <string>
#include "Staff.h"

/**
 * @brief Represents a Team in the staff hierarchy, which can contain multiple Staff members
 */

class Team : public Staff {

private:
	std::string name;
	std::string title;
	int rank = 0;
	std::vector<Staff*> members;

public:
	/**
	 * @brief Constructs a Team with a name, title and rank
	 * @param name Name of the team
	 * @param title Title of the team
	 * @param rank Rank assigned to the team
	 */
	Team(std::string name, std::string title, int rank);
	/**
     * @brief Destroys the Team and its contained Staff members
     * Deletes the Staff members owned by the Team
     */
	~Team() override;

	/**
     * @brief Gets the name of the Team
     * @return The Team's name
     */
	std::string getName() override;

	/**
     * @brief Gets the title of the Team
     * @return The Team's title
     */
	std::string getTitle() override;

	/**
     * @brief Performs the Team's operation
     * Currently has an empty implementation
     */
	void operation();

	/**
     * @brief Prints the Team and its Staff hierarchy
     * Recursively prints the details of the Staff members contained in the Team
     * @param indent String used for indentation
     */
	void printHierarchy(std::string indent) override;

	/**
     * @brief Gets the authority rank of the Team
     * @return The Team's rank
     */
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
     * @brief The team's direct members, in the order they were added
     * @return The members (owned by the team)
     */
    const std::vector<Staff*>& getMembers() const;

	/**
     * @brief Attempts to handle a request based on the required rank
     * @param rank Minimum rank required to handle the request
     * @return true if handled, otherwise false
     */
    bool handleRequest(int rank) override;
};

#endif
