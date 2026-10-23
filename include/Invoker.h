#ifndef INVOKER_H
#define INVOKER_H

class Command;
class Memento;

/**
 * @brief Invoker (Command pattern) used by the WorkflowManager.
 * Holds the command to run and triggers it, without knowing which action it
 * is. Before a command runs, a Memento of the receiver can be saved so that
 * the action can be rolled back (Memento pattern collaboration).
 * The command is only referenced (aggregation), never owned.
 */
class Invoker {
public:
    Invoker();
    ~Invoker();
    /** @brief Choose the command to run next (not owned). */
    void setCommand(Command* c);
    /** @brief Execute the current command on its receiver. */
    void executeCommand();
    /**
     * @brief Save a snapshot taken before execute() so the action can be undone.
     * @param m The memento created by the receiver (see CommandHistory).
     */
    void save(Memento* m);
    /**
     * @brief Undo the last command, restoring the receiver from a snapshot.
     * @param m The memento to restore.
     */
    void undo(Memento* m);
private:
    Command* command;
};

#endif // INVOKER_H
