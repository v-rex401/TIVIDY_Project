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
class AssignmentStrategy;
class RoutingStrategy;

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
	std::vector<WorkflowInstance*> instances;      ///< Every instance created so far (owned).
	std::vector<WorkflowDefinition*> instanceDefinitions; ///< Each instance's private copy of its definition (owned), same index as instances.
	int nextInstanceNumber;                        ///< Number used in the next instance id ("WF-<n>").

	/**
	 * @brief Whether an instance was created by this facade.
	 * @param instance The instance to look for.
	 * @return True if the facade owns it.
	 */
	bool owns(WorkflowInstance* instance) const;

public:
	/** @brief Create the facade together with the WorkflowManager it owns. */
	WorkflowFacade();

	/** @brief Delete the owned WorkflowManager and every owned definition and instance. */
	~WorkflowFacade();

	/**
	 * @brief Create a new instance of a definition and start it (Pending -> Active).
	 *
	 * The new instance is the last one in getInstances().
	 *
	 * @param definition The definition to run (not owned). Null is ignored.
	 */
	void startWorkflow(WorkflowDefinition* definition);

	/**
	 * @brief Create a new, independent instance of a definition.
	 *
	 * The instance works on its own clone of the definition (Prototype), so
	 * work done in one instance never shows up in another. It starts in
	 * Pending with the id "WF-<n>", numbered from 1.
	 *
	 * @param definition The definition to instantiate (not owned).
	 * @return The new instance (owned by the facade), or nullptr if definition is null.
	 */
	WorkflowInstance* createWorkflowInstance(WorkflowDefinition* definition);

	/**
	 * @brief Submit a work item of an instance for assignment and routing.
	 *
	 * The assignment strategy picks who is responsible and the routing
	 * strategy picks the Stage it goes to. On success the item is in
	 * instance->getWorkItems() and the destination Stage owns it. If no one
	 * is assigned or no Stage is chosen, nothing changes and the caller
	 * still owns the item.
	 *
	 * @param instance The instance the item belongs to. Null or not created by this facade is ignored.
	 * @param workitem The item to submit (must be heap-allocated).
	 */
	void submitWorkItem(WorkflowInstance* instance, WorkItem* workitem);

	/**
	 * @brief Run an instance forward through its definition.
	 *
	 * Starts the instance if it has not been started yet, then lets its
	 * current State move it on. A Pending or Active instance ends up
	 * Completed; a Completed or Cancelled one does not change.
	 *
	 * @param w The instance to execute. Null or not created by this facade is ignored.
	 */
	void executeWorkflow(WorkflowInstance* w);

	/**
	 * @brief Cancel a running instance.
	 * @param instance The instance to cancel. Null or not created by this facade is ignored.
	 */
	void cancelWorkflow(WorkflowInstance* instance);

	/**
	 * @brief Choose how submitted work is assigned to staff.
	 * @param strategy The strategy to use; the manager takes ownership and deletes the previous one.
	 */
	void setAssignmentStrategy(AssignmentStrategy* strategy);

	/**
	 * @brief Choose how submitted work is routed to a Stage.
	 * @param strategy The strategy to use; the manager takes ownership and deletes the previous one.
	 */
	void setRoutingStrategy(RoutingStrategy* strategy);

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
	 * @brief Every instance created so far, for the TUI to list and pick from.
	 * @return All instances in creation order, including finished ones (owned by the facade).
	 */
	std::vector<WorkflowInstance*> getInstances();

	/**
	 * @brief Find an instance by its id.
	 * @param id The instance id.
	 * @return The instance, or nullptr if there is none with that id.
	 */
	WorkflowInstance* getInstance(const std::string& id);

	WorkflowFacade(const WorkflowFacade&) = delete;            ///< Not copyable: owns the manager.
	WorkflowFacade& operator=(const WorkflowFacade&) = delete; ///< Not copyable: owns the manager.
};

#endif // WORKFLOWFACADE_H
