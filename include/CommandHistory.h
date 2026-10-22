#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

class CommandHistory {

private:
	vector<Memento*> history;

public:
	void save(Memento* m);

	Memento* restore();

	void ~CommandHistory();
};

#endif
