#include "WorkflowEvent.h"
#include <algorithm>
#include "Observer.h"
#include "WorkflowInstance.h"
#include "EventLog.h"

// Added missing constructor
WorkflowInstance::WorkflowInstance()
{
}
Memento *WorkflowInstance::createMemento()
{
    // TODO - implement WorkflowInstance::createMemento
    return nullptr;
}

void WorkflowInstance::setMemento(Memento * /* m */)
{
    // TODO - implement WorkflowInstance::setMemento
    return;
}

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

void WorkflowInstance::detach(Observer *o)
{
    std::vector<Observer *> &observerList = getObserverList();
    observerList.erase(std::remove(observerList.begin(), observerList.end(), o), observerList.end());
}

void WorkflowInstance::setState(State * /* s */)
{
    // TODO - implement WorkflowInstance::setState
    return;
}

State *WorkflowInstance::getState()
{
    return currentState;
}

std::vector<WorkItem *> WorkflowInstance::getWorkItem() const
{
    return workItem;
}

void WorkflowInstance::addWorkItem(WorkItem* item)
{
    if (item != nullptr && std::find(workItem.begin(), workItem.end(), item) == workItem.end()) {
        workItem.push_back(item);
    }
}

double WorkflowInstance::getTransactionValue() const { return transactionValue; }
void WorkflowInstance::setTransactionValue(double value) { transactionValue = value; }
double WorkflowInstance::getEventMetric() const { return eventMetric; }
void WorkflowInstance::setEventMetric(double value) { eventMetric = value; }

EventLog *WorkflowInstance::getEventLog()
{
    // TODO - implement WorkflowInstance::getEventLog
    return nullptr;
}

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

void WorkflowInstance::handle()
{
    // TODO - implement WorkflowInstance::handle
    return;
}

void WorkflowInstance::accept(WorkflowVisitor * /* v */)
{
    // TODO - implement WorkflowInstance::accept
    return;
}

void WorkflowInstance::setId(const std::string &newId)
{
    id = newId;
}
void WorkflowInstance::setDefinition(WorkflowDefinition *def)
{
    definition = def;
}