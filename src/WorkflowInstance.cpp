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

//Added a helper for the time
static WorkflowEvent* makeStateChangeEvent(const std::string& id) {
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
WorkflowInstance::~WorkflowInstance() {
    delete history;
    delete eventLog;
}

/**
 * @brief Restores the state saved in a Memento. It does not go through setState(), as the state is already owned by this instance.
 * @param m The Memento to restore from.
 */
void WorkflowInstance::setMemento(Memento *m) {
    if(m == nullptr) {
        return;
    }

    State* saved = m->getState(); //friend access
    if(saved != nullptr) {
        currentState = saved;
        eventLog->addEntry(makeStateChangeEvent(id));
        notify();
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
void WorkflowInstance::setState(State *s) {
    if(s == nullptr) {
        return;
    }
    ownedStates.push_back(std::unique_ptr<State>(s));
    currentState = s;

    eventLog->addEntry(makeStateChangeEvent(id)); //the log owns the event
    notify();
}

/**
 * @brief a getter function
 * @return the current state.
 */
State *WorkflowInstance::getState() {
    return currentState;
}

/**
 * @brief a getter function
 * @return the work item.
 */
std::vector<WorkItem *> WorkflowInstance::getWorkItem() {
    return workItem;
}

/**
 * @brief a getter function
 * @return the event log.
 */
EventLog *WorkflowInstance::getEventLog() {
    return eventLog;
}

/**
 * @brief Records a state change event
 */
void WorkflowInstance::notify()
{
    // most recent entry of log is the announced event.
    EventLog *log = getEventLog();
    if (log == nullptr)
    {
        return;
    }
    std::vector<WorkflowEvent *> entries = log->getEntries();
    if (entries.empty())
    {
        return;
    }
    WorkflowEvent *latest = entries.back();
    // use copy to iterate, may detach during update
    std::vector<Observer *> snapshot = getObserverList();
    for (std::size_t i = 0; i < snapshot.size(); ++i)
    {
        snapshot[i]->update(latest, this);
    }
}

/**
 * @brief Delegates to the current state
 */
void WorkflowInstance::handle() {
    if(currentState != nullptr) {
        currentState->handle(this);
    }
}

void WorkflowInstance::accept(WorkflowVisitor *v)
{
    // TODO - implement WorkflowInstance::accept
    return;
}
