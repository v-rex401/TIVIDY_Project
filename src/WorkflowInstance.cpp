/**
 * @file WorkflowInstance.cpp
 * @brief Implementation of WorkflowInstance with State context, Memento originator, Observer subject and Visitor.
 * @author Jezelle Govender
 * @date 2026-10-25
 */

#include "WorkflowInstance.h"
#include "Pending.h"
#include "Observer.h"
#include "WorkItem.h"
#include "WorkflowVisitor.h"
#include "WorkflowEvent.h"
#include <algorithm>
#include "Observer.h"
#include "EventLog.h"
#include "CommandHistory.h"
#include <ctime>

// Added a helper for the time
static WorkflowEvent *makeStateChangeEvent(const std::string &id)
{
    char buf[32];
    std::time_t now = std::time(nullptr);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return new WorkflowEvent("STATE_CHANGE", "Workflow instance " + id + " changed state", buf);
}

/**
 * @brief Constructs a new WorkflowInstance. Every new instance starts in the Pending state, and it owns its own EventLog and CommandHistory.
 */
WorkflowInstance::WorkflowInstance()
    : id(""),
    currentState(nullptr),
    definition(nullptr),
    history(new CommandHistory()),
    eventLog(new EventLog()) {
        State* initial = new Pending(); //not through setState because no observers yet attached, so no event is needed.
        ownedStates.push_back(std::unique_ptr<State>(initial));
        currentState = initial;
}

/**
 * @brief Captures a snapshot of the current state.
 * The Memento only points at the state, and the instance keeps ownership of it.
 * @return A newly allocated Memento.
 */
Memento *WorkflowInstance::createMemento() {
    Memento* m = new Memento();
    m->setState(currentState);
    m->workItem = workItem.empty() ? nullptr : workItem.front();
    return m;
}

/**
 * @brief Destroys the instance. The owned states are automatically released by the unique_ptr vector.
 */
WorkflowInstance::~WorkflowInstance()
{
    delete history;
    delete eventLog;
}

/**
 * @brief Restores the state saved in a Memento. It does not go through setState(), as the state is already owned by this instance.
 * @param m The Memento to restore from.
 */
void WorkflowInstance::setMemento(Memento *m)
{
    if (m == nullptr)
    {
        return;
    }

    State *saved = m->getState(); // friend access
    if (saved != nullptr)
    {
        currentState = saved;
        
        WorkflowEvent* e = makeStateChangeEvent(id);
        eventLog->addEntry(e);
        notify(e);
    }
}

/**
 * @brief Attaches an observer, uses a reference to the real list.
 */
void WorkflowInstance::attach(Observer *o)
{
    std::vector<Observer *> &observerList = getObserverList();
    if (o == nullptr)
    {
        return;
    }
    if (std::find(observerList.begin(), observerList.end(), o) != observerList.end())
    {
        return; // already attached
    }
    observerList.push_back(o);
}

/**
 * @brief Detaches an observer.
 */
void WorkflowInstance::detach(Observer *o)
{
    std::vector<Observer *> &observerList = getObserverList();
    observerList.erase(std::remove(observerList.begin(), observerList.end(), o), observerList.end());
}

/**
 * @brief Changes the current state and notifies the observers. Takes ownership of s, which must be a new state created by the caller.
 * To go back to an old state, use setMemento().
 */
void WorkflowInstance::setState(State *s)
{
    if (s == nullptr)
    {
        return;
    }
    ownedStates.push_back(std::unique_ptr<State>(s));
    currentState = s;

    WorkflowEvent* e = makeStateChangeEvent(id);
    eventLog->addEntry(e);   // the log owns the event

    notify(e); //this now logs the event once, and tells the observers
}

/**
 * @brief a getter function
 * @return the current state.
 */
State *WorkflowInstance::getState()
{
    return currentState;
}

/**
 * @brief a getter function
 * @return the work item.
 */
std::vector<WorkItem *> WorkflowInstance::getWorkItem()
{
    return workItem;
}

const std::vector<WorkItem*>& WorkflowInstance::getWorkItems() const
{
    return workItem;
}

void WorkflowInstance::addWorkItem(WorkItem* item)
{
    if (item != nullptr && std::find(workItem.begin(), workItem.end(), item) == workItem.end())
    {
        workItem.push_back(item);
    }
}

void WorkflowInstance::setData(const std::string& key, double value)
{
    this->data[key] = value;
}

double WorkflowInstance::getData(const std::string& key, double fallback) const
{
    std::map<std::string, double>::const_iterator it = this->data.find(key);
    if (it == this->data.end())
    {
        return fallback;
    }
    return it->second;
}

/**
 * @brief a getter function
 * @return the event log.
 */
EventLog *WorkflowInstance::getEventLog()
{
    return eventLog;
}

void WorkflowInstance::notify(WorkflowEvent *e){
    if (e == nullptr){
        return;
    }
    // Iterate a copy: an observer may detach itself during update().
    std::vector<Observer *> snapshot = getObserverList();
    for (Observer *o : snapshot)
    {
        if (o != nullptr)
        {
            o->update(e, this);
        }
    }
}

/**
 * @brief Delegates to the current state
 */
void WorkflowInstance::handle()
{
    if (currentState != nullptr)
    {
        currentState->handle(this);
    }
}

/**
 * @brief Lets a visitor walk this instance's structure, so walk through work items, participants and history entries.
 */
void WorkflowInstance::accept(WorkflowVisitor *v)
{
    if (v == nullptr)
    {
        return;
    }

    for (std::size_t i = 0; i < workItem.size(); ++i)
    {
        workItem[i]->accept(v);
    }

    for (std::size_t i = 0; i < participants.size(); ++i)
    {
        v->visitParticipant(participants[i]);
    }

    std::vector<WorkflowEvent *> entries = eventLog->getEntries();
    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        v->visitHistoryEntry(entries[i]);
    }
}

std::string WorkflowInstance::getId() const
{
    return id;
}

void WorkflowInstance::setId(const std::string &newId)
{
    this->id = newId;
}

void WorkflowInstance::setDefinition(WorkflowDefinition *def)
{
    this->definition = def;
}