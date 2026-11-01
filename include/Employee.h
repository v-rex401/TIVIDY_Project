#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <string>
#include "Staff.h"

/**
 * @brief Represents an Employee in the staff hierarchy
 */

class Employee : public Staff {

private:
	std::string name;
	std::string title;
	int rank = 0;

public:
	/**
	 * @brief The default constructor for an Employee.
	 */
	Employee() = default;

	/**
     * @brief Constructs an Employee with a name, title and rank.
     */
    Employee(std::string name, std::string title, int rank);

	/**
     * @brief Destructor for the Employee
     */
	~Employee() override = default;

	/**
     * @brief Gets the name of the Employee
     * @return The Employee's name
     */
	std::string getName() override;

	/**
     * @brief Gets the job title of the Employee
     * @return The Employee's title
     */
	std::string getTitle() override;

	/**
     * @brief Gets the authority rank of the Employee
     * @return The Employee's rank
     */
	int getRank() override;

	/**
     * @brief Prints the Employee's details in the staff hierarchy
     * @param indent String used for indentation
     */
    void printHierarchy(std::string indent) override;

	/**
     * @brief Attempts to handle a request based on the required rank
     * @param rank Minimum rank required to handle the request
     * @return true if handled, otherwise false
     */
    bool handleRequest(int rank) override;
};

#endif
