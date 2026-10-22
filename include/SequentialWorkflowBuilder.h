#ifndef SEQUENTIALWORKFLOWBUILDER_H
#define SEQUENTIALWORKFLOWBUILDER_H

#include <string>

#include "WorkflowBuilder.h"

class WorkflowDefinition;

class SequentialWorkflowBuilder : public WorkflowBuilder {

private:
	WorkflowDefinition* definition;

public:
	void buildDefinition(std::string name);

	void buildStage(std::string name);

	void buildTask(std::string stageName, std::string title);

	void linkHandlers();

	void buildTransition(std::string from, std::string to);

	void buildParticipantSlot(std::string stageName, std::string role);

	void setAssignmentStrategyType(std::string type);

	void setRoutingStrategyType(std::string type);

	void setInitialData(std::string key, std::string value);

	WorkflowDefinition* getWorkflowDefinition();
};

#endif
