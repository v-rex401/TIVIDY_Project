#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <vector>

class Memento;
/**
 * @brief Caretaker (Memento pattern): keeps the saved snapshots of a
 * WorkflowInstance so that it can be rolled back.
 * Owned by the WorkflowInstance (composition in the UML). It stores the
 * mementos but never looks inside them.
 * Ownership: save() takes ownership of the memento and the destructor
 * releases them. Copying is disabled to prevent double deletion.
 */
class CommandHistory {
public:
    CommandHistory();
    ~CommandHistory();
    CommandHistory(const CommandHistory&) = delete;
    CommandHistory& operator=(const CommandHistory&) = delete;
    /** @brief Store a snapshot (ownership passes to this history). */
    void save(Memento* m);
    /**
     * @brief Most recently saved snapshot.
     * @return The latest memento, or nullptr if nothing has been saved.
     */
    Memento* restore();
private:
    std::vector<Memento*> history;
};

#endif // COMMANDHISTORY_H
