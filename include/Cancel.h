#ifndef CANCEL_H
#define CANCEL_H

#include "Command.h"
/**
 * @brief ConcreteCommand: Cancel the workflow instance (such as emergency, retries exhausted or approval unresolved)
 * Receiver is the WorkflowInstance passed at construction (not owned).
 */
class Cancel : public Command {
public:
    /** @param receiver The WorkflowInstance this command acts on (not owned). */
    explicit Cancel(WorkflowInstance* receiver);
    ~Cancel();
    /** @brief Carry out this action on the receiver. */
    void execute() override;
    /** @brief Reverse this action on the receiver where possible. */
    void undo() override;
};

#endif // CANCEL_H
