#ifndef STAFF_H
#define STAFF_H
#include <string>

class Staff
{

private:
	Staff *successor;

public:
	/**
	 * @brief Virtual destructor the Staff interface.
	 * It ensures proper cleanup of derived Employee and Team objects when deleted via a Staff pointer.
	 */
	virtual ~Staff() = default;
	virtual std::string getName() = 0;
	virtual std::string getTitle() = 0;
	virtual int getRank() = 0;
	virtual void printHierarchy(std::string) {}
};

#endif
