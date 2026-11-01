#ifndef EXTERNALSYSTEM_H
#define EXTERNALSYSTEM_H

/**
 * @brief Target of the Adapter pattern.
 *
 * WorkflowManager holds an ExternalSystem* and never names a concrete service,
 * so a new provider is added by writing one adapter and changing nothing else.
 */

class ExternalSystem {


public:
	virtual ~ExternalSystem();

	/** @brief Perform the outbound call in whatever form the concrete system needs. */
	virtual void request() = 0;
};

#endif
