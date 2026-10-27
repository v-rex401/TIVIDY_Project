/**
 * @file WorkflowFacade.h
 * @brief Single entry point the client (TUI) uses to drive the workflow engine.
 * @author JP
 * @date 2026-10-22
 */

#ifndef WORKFLOWFACADE_H
#define WORKFLOWFACADE_H

#include <string>
#include <vector>

class WorkflowManager;
class WorkflowDefinition;
class WorkflowInstance;
class WorkItem;

/**
 * @brief Facade participant of the Facade pattern.
 *
 * Gives the client one simple interface to the workflow subsystem
 * (manager, instances, strategies, states) and delegates every call to
 * the WorkflowManager. It holds no workflow logic of its own.
 *
 * gof roles:
 *  - Facade     : WorkflowFacade
 *  - Subsystem  : WorkflowManager (and the classes it coordinates)
 *  - Client     : Client (the TUI)
 *
 * The facade creates and owns its WorkflowManager, and it owns every
 * definition and instance the client works with: the manager runs
 * instances but never deletes them. So the facade is not copyable.
 */
class WorkflowFacade {

private:
	WorkflowManager* manager;                      ///< Subsystem every call is delegated to (owned).
	std::vector<WorkflowDefinition*> definitions;  ///< Registered definitions (owned).

public:
	/** @brief Create the facade together with the WorkflowManager it owns. */
	WorkflowFacade();

	/** @brief Delete the owned WorkflowManager and every owned definition. */
	~WorkflowFacade();

	/**
	 * @brief Start a workflow from a definition.
	 * @param definition The definition to run (not owned).
	 */
	void startWorkflow(WorkflowDefinition* definition);

	/**
	 * @brief Create a new, independent instance of a definition.
	 * @param definition The definition to instantiate (not owned).
	 * @return The new instance; the manager owns it.
	 */
	WorkflowInstance* createWorkflowInstance(WorkflowDefinition* definition);

	/**
	 * @brief Submit a work item of an instance for assignment and routing.
	 * @param instance The instance the item belongs to.
	 * @param workitem The item to submit.
	 */
	void submitWorkItem(WorkflowInstance* instance, WorkItem* workitem);

	/**
	 * @brief Run an instance forward through its definition.
	 * @param w The instance to execute.
	 */
	void executeWorkflow(WorkflowInstance* w);

	/**
	 * @brief Cancel a running instance.
	 * @param instance The instance to cancel.
	 */
	void cancelWorkflow(WorkflowInstance* instance);

	/**
	 * @brief Register a definition the client can start instances from.
	 * @param definition The definition to register; the facade takes ownership.
	 *        A null or already registered definition is ignored.
	 */
	void addDefinition(WorkflowDefinition* definition);

	/**
	 * @brief Definitions available to start.
	 * @return The registered definitions, in the order they were added (owned by the facade).
	 */
	std::vector<WorkflowDefinition*> getDefinitions();

	/**
	 * @brief Live instances, for the TUI to list and pick from (UML-018).
	 * @return All instances (owned by the manager).
	 */
	std::vector<WorkflowInstance*> getInstances();

	/**
	 * @brief Find an instance by its id (UML-018).
	 * @param id The instance id.
	 * @return The instance, or nullptr if there is none with that id.
	 */
	WorkflowInstance* getInstance(const std::string& id);

	WorkflowFacade(const WorkflowFacade&) = delete;            ///< Not copyable: owns the manager.
	WorkflowFacade& operator=(const WorkflowFacade&) = delete; ///< Not copyable: owns the manager.
};

#endif // WORKFLOWFACADE_H
