#ifndef INVOKER_H
#define INVOKER_H

class Command;
class CommandHistory;
class Memento;

/**
 * @brief Invoker (Command pattern) used by the WorkflowManager.
 * Holds the command to run and triggers it, without knowing which action it
 * is. Before a command runs, a Memento of the receiver can be saved into a
 * CommandHistory so that the action can be rolled back (Memento pattern
 * collaboration).
 * The command and the history are only referenced (aggregation), never owned.
 */
class Invoker {
public:
    /**
     * @param history Where saved snapshots are kept (not owned). May be
     *                nullptr if snapshots are not needed.
     */
    explicit Invoker(CommandHistory* history = nullptr);
    ~Invoker();

    /** @brief Choose the command to run next (not owned). */
    void setCommand(Command* c);
    /** @brief Execute the current command on its receiver. Does nothing if none is set. */
    void executeCommand();
    /**
     * @brief Keep a snapshot taken before execute() so the action can be undone.
     *
     * Ownership of @p m passes to the history. If no history is attached the
     * memento cannot be kept, so it is deleted.
     */
    void save(Memento* m);
    /**
     * @brief Roll the current command's receiver back to a snapshot.
     * @param m The snapshot to restore. If nullptr, the most recent snapshot
     *          in the history is used. Does nothing if there is none.
     */
    void undo(Memento* m);

private:
    Command* command;
    CommandHistory* history;
};

#endif // INVOKER_H
