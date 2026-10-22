#ifndef COMMAND_H
#define COMMAND_H

class Command {

private:
	WorkflowInstance* receiver;

public:
	virtual void execute() = 0;

	virtual void undo() = 0;
};

#endif
