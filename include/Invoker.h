#ifndef INVOKER_H
#define INVOKER_H

class Invoker {

private:
	Command* command;

public:
	void setCommand(Command* c);

	void executeCommand();

	void save(Memento* m);

	void undo(Memento* m);
};

#endif
