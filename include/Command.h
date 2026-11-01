#ifndef COMMAND_H
#define COMMAND_H

class WorkflowInstance;
class Memento;

/**
 * @brief Abstract Command (Command pattern).
 * Encapsulates one workflow action (approve, reject, retry, cancel) as an
 * object so that it can be executed and undone later.
 * Pattern roles:
 *  - Command          : this class
 *  - ConcreteCommand  : Retry, Reject, Approve, Cancel
 *  - Invoker          : Invoker (driven by the WorkflowManager)
 *  - Receiver         : WorkflowInstance
 * The receiver is only referenced, never owned: the WorkflowInstance outlives
 * the commands that act on it.
 * Undo: a concrete command calls saveSnapshot() at the start of execute() and
 * restoreSnapshot() in undo(), so undo() returns the receiver to the state it
 * was in just before this command ran.
 */
class Command {
public:
    /**
     * @param receiver The WorkflowInstance this command acts on (not owned).
     */
    explicit Command(WorkflowInstance* receiver);
    virtual ~Command();

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    /** @brief Perform the action on the receiver. */
    virtual void execute() = 0;
    /** @brief Reverse the effect of execute(). */
    virtual void undo() = 0;
    /** @brief The instance this command acts on (the Invoker needs it to roll back). */
    WorkflowInstance* getReceiver() const;
protected:
    /** @brief Capture the receiver's current state (replaces any earlier snapshot). */
    void saveSnapshot();
    /** @brief Put the receiver back to the captured state; does nothing if none was taken. */
    void restoreSnapshot();

private:
    WorkflowInstance* receiver;
    Memento* snapshot;   ///< owned by this command
};

#endif // COMMAND_H
