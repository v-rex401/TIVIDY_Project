#ifndef APPROVE_H
#define APPROVE_H

#include "Command.h"

class Approve : public Command
{

public:
	void execute();

	void undo();
};

#endif
