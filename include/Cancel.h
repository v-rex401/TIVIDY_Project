#ifndef CANCEL_H
#define CANCEL_H

class Cancel : Command {


public:
	void execute();

	void undo();
};

#endif
