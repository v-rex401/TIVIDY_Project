#ifndef COMMAND_H
#define COMMAND_H

class WorkflowInstance;
/**
 * @brief Command (Command pattern).
 *
 * encapsulates one workflow action (approve, reject, retry, cancel) as an
 * object so that it can be executed and undone later.
 *
 * gof roles:
 *  - Command          : Command
 *  - ConcreteCommand  : Retry, Reject, Approve, Cancel
 *  - Invoker          : Invoker (driven by the WorkflowManager)
 *  - Receiver         : WorkflowInstance
 *
 * The receiver is only referenced, never owned: the WorkflowInstance outlives
 * the commands that act on it.
 */
class Command {
public:

    /**
     * @param receiver The WorkflowInstance this command acts on (not owned).
     */
    explicit Command(WorkflowInstance* receiver);
    virtual ~Command();
    /** @brief Perform the action on the receiver. */
    virtual void execute() = 0;
    /** @brief Reverse the effect of execute(), where that is sensible. */
    virtual void undo() = 0;

protected:

    /** @brief Access for concrete commands (the member itself stays private, as in the UML). */
    WorkflowInstance* getReceiver() const;
private:
    WorkflowInstance* receiver;
};

#endif // COMMAND_H
