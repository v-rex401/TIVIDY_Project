#ifndef RETRY_H
#define RETRY_H

class Retry : Command {


public:
	void execute();

	void undo();
};

#endif
