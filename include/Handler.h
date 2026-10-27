#ifndef HANDLER_H
#define HANDLER_H

#include "WorkItem.h"
#include "Staff.h"
#include <vector>
#include <string>

class Handler : public WorkItem
{

private:
	std::vector<WorkItem *> members;
	std::string name;
	std::string title;
	Staff *responsible;
	std::string severity;
	std::string requiredPosition;

public:
	Handler(std::string name, std::string title);

	std::string getName() override;

	std::string getTitle() override;

	void printHierarchy(std::string indent) override;

	void addMember(WorkItem* Component);

	void removeMember(WorkItem* Component);

	void setResponsible(Staff* staff) override;

	bool handleRequest(int rank) override;

	WorkItem* clone() override;

	std::string getSeverity() override;

	std::string getRequiredPosition() override;

	void setRequiredPosition(const std::string& position) override;

	Staff* getResponsible() override;

	~Handler() override;
};

#endif
