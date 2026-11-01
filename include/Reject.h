#ifndef REJECT_H
#define REJECT_H

#include "Command.h"
/**
 * @brief ConcreteCommand: Reject a pending approval so that the work is sent back for reassessment
 * Receiver is the WorkflowInstance passed at construction (not owned).
 */
class Reject : public Command {
public:
    /** @param receiver The WorkflowInstance this command acts on (not owned). */
    explicit Reject(WorkflowInstance* receiver);
    ~Reject();
    /** @brief Carry out this action on the receiver. */
    void execute() override;
    /** @brief Reverse this action on the receiver where possible. */
    void undo() override;
};

#endif // REJECT_H
