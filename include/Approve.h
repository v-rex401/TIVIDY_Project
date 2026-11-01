#ifndef APPROVE_H
#define APPROVE_H

#include "Command.h"
/**
 * @brief ConcreteCommand: Approve a pending request so that the workflow can continue.
 * Receiver is the WorkflowInstance passed at construction (not owned).
 */
class Approve : public Command {
public:
    /** @param receiver The WorkflowInstance this command acts on (not owned). */
    explicit Approve(WorkflowInstance* receiver);
    ~Approve();
    /** @brief Carry out this action on the receiver. */
    void execute() override;
    /** @brief Reverse this action on the receiver where possible. */
    void undo() override;
};

#endif // APPROVE_H
