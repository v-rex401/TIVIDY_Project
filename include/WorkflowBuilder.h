#ifndef WORKFLOWBUILDER_H
#define WORKFLOWBUILDER_H

#include <string>

class WorkflowBuilder {


public:
	virtual void buildStage(std::string name) = 0;

	virtual void buildTask(std::string stageName, std::string title) = 0;

	virtual void linkHandlers() = 0;

	void buildTransition(std::string from, std::string to);

	void buildParticipantSlot(std::string stageName, std::string role);

	void setAssignmentStrategyType(std::string type);

	void setRoutingStrategyType(std::string type);

	void setInitialData(std::string key, std::string value);

	void buildDefinition(std::string name);

	virtual ~WorkflowBuilder()  = default;
};

#endif
