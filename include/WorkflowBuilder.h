#ifndef WORKFLOWBUILDER_H
#define WORKFLOWBUILDER_H

class WorkflowBuilder {


public:
	virtual void buildStage(string name) = 0;

	virtual void buildTask(string stageName, string title) = 0;

	virtual void linkHandlers() = 0;

	void buildTransition(string from, string to);

	void buildParticipantSlot(string stageName, string role);

	void setAssignmentStrategyType(string type);

	void setRoutingStrategyType(string type);

	void setInitialData(string key, string value);

	void buildDefinition(string name);
};

#endif
