#ifndef SEQUENTIALWORKFLOWBUILDER_H
#define SEQUENTIALWORKFLOWBUILDER_H

class SequentialWorkflowBuilder : WorkflowBuilder {

public:
	WorkflowDefinition* definition;

	void buildDefinition(string name);

	void buildStage(string name);

	void buildTask(string stageName, string title);

	void linkHandlers();

	void buildTransition(string from, string to);

	void buildParticipantSlot(string stageName, string role);

	void setAssignmentStrategyType(string type);

	void setRoutingStrategyType(string type);

	void setInitialData(string key, string value);

	WorkflowDefinition* getWorkflowDefinition();
};

#endif
