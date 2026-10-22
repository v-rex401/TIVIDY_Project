#ifndef STAFF_H
#define STAFF_H
#include <string>

class Staff {

private:
	Staff* successor;

public:
	virtual ~Staff() = default;
};


#endif
