#ifndef STAFF_H
#define STAFF_H
#include <string>

/**
 * @brief Abstract base class representing a staff member in a chain of responsibility
 */

class Staff
{

private:
	Staff *successor = nullptr;

public:
    /**
     * @brief Get the next staff member in the chain
     * @return The successor, or nullptr at the end of the chain.
     *
     * Public so a client can show who a request would be passed to.
     */
    Staff* getNextHandler() const
    {
        return this->successor;
    }

	/**
	 * @brief Virtual destructor the Staff interface.
	 * It ensures proper cleanup of derived Employee and Team objects when deleted via a Staff pointer.
	 */
	virtual ~Staff() = default;

    /**
     * @brief Gets the name of the Staff member
     * @return The Staff member's name
     */
	virtual std::string getName() = 0;

    /**
     * @brief Gets the title of the Staff member
     * @return The Staff member's title
     */
	virtual std::string getTitle() = 0;

    /**
     * @brief Gets the authority rank of the Staff member
     * @return The Staff member's rank
     */
	virtual int getRank() = 0;

    /**
     * @brief Prints the Staff hierarchy
     * The default implementation performs no action
     * @param indent String used for indentation
     */
	virtual void printHierarchy(std::string indent) {}

	/**
     * @brief Sets the next staff member in the chain
     * @param next Pointer to the next Staff handler
     */
    void setNextHandler(Staff* next)
    {
        this->successor = next;
    }

	/**
     * @brief Attempts to handle a request based on required rank
     * @param rank Minimum rank required to handle the request
     * @return true if handled, otherwise false.
     */
    virtual bool handleRequest(int rank) = 0;
};

#endif
