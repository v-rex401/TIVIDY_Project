#ifndef RETRY_H
#define RETRY_H

#include "Command.h"
/**
 * @brief ConcreteCommand: Retry a failed work item or operation (e.g. a failed laboratory test) while retries remain
 * Receiver is the WorkflowInstance passed at construction (not owned).
 */
class Retry : public Command {
public:
    /** @param receiver The WorkflowInstance this command acts on (not owned). */
    explicit Retry(WorkflowInstance* receiver);
    ~Retry();
    /** @brief Carry out this action on the receiver. */
    void execute() override;
    /** @brief Reverse this action on the receiver where possible. */
    void undo() override;
};

#endif // RETRY_H
