/**
 * @file Session.cpp
 * @brief Implementation of Session: every TUI action, run against the real engine.
 * @author JP
 * @date 2026-10-31
 */

#include "Session.h"

#include <algorithm>
#include <climits>
#include <functional>
#include <sstream>

#include "Approve.h"
#include "ApprovalChainRouting.h"
#include "AssignmentRule.h"
#include "AuditExportVisitor.h"
#include "Cancel.h"
#include "Clock.h"
#include "CommandHistory.h"
#include "CoutCapture.h"
#include "Employee.h"
#include "EscalatingWorkflowBuilder.h"
#include "EventBasedRouting.h"
#include "EventLog.h"
#include "Invoker.h"
#include "Memento.h"
#include "ProgressReportVisitor.h"
#include "Reject.h"
#include "Retry.h"
#include "RoleBasedAssignment.h"
#include "RuleBasedAssignment.h"
#include "SequentialWorkflowBuilder.h"
#include "SeverityBasedRouting.h"
#include "Stage.h"
#include "Task.h"
#include "Team.h"
#include "Threshold.h"
#include "WorkflowDefinition.h"
#include "WorkflowDirector.h"
#include "WorkflowEvent.h"
#include "WorkflowInstance.h"
#include "WorkflowRepository.h"
#include "WorkloadBasedAssignment.h"

namespace
{
    const std::size_t kMaxOutputLines = 2000; ///< Oldest engine output is dropped past this.

    /** @return The time of day now, "HH:MM:SS". */
    std::string timeNow()
    {
        std::string stamp = Clock::now(); // "YYYY-MM-DD HH:MM:SS"
        return stamp.size() >= 19 ? stamp.substr(11, 8) : stamp;
    }

    /**
     * @brief Format a number without trailing zeros, e.g. 12000 or 7.5.
     * @param value The number.
     * @return The text.
     */
    std::string number(double value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }

    /**
     * @brief The state a STATE_CHANGE description moved to.
     * @param description e.g. "Active -> Pending (restored) [WF-1]".
     * @return e.g. "Pending", or empty if it is not a state change.
     */
    std::string targetState(const std::string &description)
    {
        std::size_t arrow = description.find(" -> ");
        if (arrow == std::string::npos)
        {
            return "";
        }
        std::string to = description.substr(arrow + 4);
        std::size_t cut = to.find(" [");
        if (cut != std::string::npos)
        {
            to = to.substr(0, cut);
        }
        cut = to.find(" (restored)");
        if (cut != std::string::npos)
        {
            to = to.substr(0, cut);
        }
        return to;
    }
}

Session::Session(const ScenarioSetup &setup) : spec(setup), adapter(&notifier)
{
    {
        CoutCapture capture;
        buildStaff();
        buildDefinition();
        facade.setExternalSystem(&adapter);
        keepOutput(capture.lines());
    }
    for (const std::pair<std::string, double> &value : spec.startingData)
    {
        keys.push_back(value.first);
    }
    log(nullptr,
        "Set up \"" + spec.workflowName + "\": " + std::to_string(employees.size()) + " people, " +
            std::to_string(spec.stages.size()) + " stages, made by the " + spec.builder + " builder",
        "Builder, Composite");
}

Session::~Session()
{
    // The facade (and the instances' strategies) only point at staff, so
    // delete the cases' commands first, then the staff trees.
    records.clear();
    for (Staff *root : roots)
    {
        delete root;
    }
}

void Session::buildStaff()
{
    for (const TeamSpec &t : spec.teams)
    {
        Team *team = new Team(t.name, t.title, t.rank);
        staffByName[t.name] = team;
        Team *parent = t.parent.empty() ? nullptr : dynamic_cast<Team *>(staffByName[t.parent]);
        if (parent != nullptr)
        {
            parent->addMember(team);
        }
        else
        {
            roots.push_back(team);
        }
    }
    for (const PersonSpec &p : spec.people)
    {
        Employee *person = new Employee(p.name, p.title, p.rank);
        staffByName[p.name] = person;
        employees.push_back(person);
        Team *team = p.team.empty() ? nullptr : dynamic_cast<Team *>(staffByName[p.team]);
        if (team != nullptr)
        {
            team->addMember(person);
        }
        else
        {
            roots.push_back(person);
        }
    }

    Staff *previous = nullptr;
    for (const std::string &name : spec.chain)
    {
        std::map<std::string, Staff *>::iterator found = staffByName.find(name);
        if (found == staffByName.end())
        {
            continue;
        }
        if (previous == nullptr)
        {
            chainHead = found->second;
        }
        else
        {
            previous->setNextHandler(found->second);
        }
        previous = found->second;
    }
}

void Session::buildDefinition()
{
    static const std::map<std::string, std::function<std::unique_ptr<WorkflowBuilder>()>> builders = {
        {"Sequential", [] { return std::unique_ptr<WorkflowBuilder>(new SequentialWorkflowBuilder()); }},
        {"Escalating", [] { return std::unique_ptr<WorkflowBuilder>(new EscalatingWorkflowBuilder()); }},
    };
    std::map<std::string, std::function<std::unique_ptr<WorkflowBuilder>()>>::const_iterator make =
        builders.find(spec.builder);
    std::unique_ptr<WorkflowBuilder> builder = (make != builders.end()) ? make->second() : builders.at("Sequential")();

    std::vector<std::string> stageNames;
    std::map<std::string, std::vector<std::string>> tasks;
    for (const StageSpec &stage : spec.stages)
    {
        stageNames.push_back(stage.name);
        tasks[stage.name] = stage.tasks;
    }

    WorkflowDirector director(builder.get());
    director.construct(spec.workflowName, stageNames, tasks);
    builder->setAssignmentStrategyType(spec.assignment);
    builder->setRoutingStrategyType(spec.routing.kind);
    for (const std::pair<std::string, double> &value : spec.startingData)
    {
        builder->setInitialData(value.first, number(value.second));
    }

    blueprint = builder->getWorkflowDefinition();
    facade.addDefinition(blueprint);
}

const ScenarioSetup &Session::setup() const
{
    return spec;
}

WorkflowDefinition *Session::definition()
{
    return blueprint;
}

const std::vector<Staff *> &Session::topLevelStaff() const
{
    return roots;
}

const std::vector<Staff *> &Session::roster() const
{
    return employees;
}

ActionResult Session::addPerson(const PersonSpec &person)
{
    ActionResult result;
    if (person.name.empty() || person.title.empty())
    {
        result.message = "A new person needs a name and a title.";
        return result;
    }
    if (staffByName.count(person.name) != 0)
    {
        result.message = "There is already someone called " + person.name + ".";
        return result;
    }
    Team *team = nullptr;
    if (!person.team.empty())
    {
        std::map<std::string, Staff *>::iterator found = staffByName.find(person.team);
        team = (found != staffByName.end()) ? dynamic_cast<Team *>(found->second) : nullptr;
        if (team == nullptr)
        {
            result.message = "There is no team called " + person.team + ".";
            return result;
        }
    }

    Employee *employee = new Employee(person.name, person.title, person.rank);
    staffByName[person.name] = employee;
    employees.push_back(employee);
    if (team != nullptr)
    {
        team->addMember(employee);
    }
    else
    {
        roots.push_back(employee);
    }
    spec.people.push_back(person);

    result.changed = true;
    result.message = "Added " + person.name + " (" + person.title + ", rank " + std::to_string(person.rank) + ")" +
                     (team != nullptr ? " to " + person.team : "");
    log(nullptr, result.message, "Composite");
    return result;
}

std::vector<ChainStep> Session::chain() const
{
    std::vector<ChainStep> steps;
    for (Staff *s = chainHead; s != nullptr; s = s->getNextHandler())
    {
        ChainStep step;
        step.name = s->getName();
        step.title = s->getTitle();
        step.rank = s->getRank();
        steps.push_back(step);
    }
    return steps;
}

std::vector<std::string> Session::staffTree()
{
    CoutCapture capture;
    for (Staff *root : roots)
    {
        root->printHierarchy("");
    }
    return capture.lines();
}

WorkflowInstance *Session::openCase()
{
    WorkflowInstance *c = nullptr;
    {
        CoutCapture capture;
        facade.startWorkflow(blueprint);
        std::vector<WorkflowInstance *> all = facade.getInstances();
        c = all.empty() ? nullptr : all.back();
        if (c != nullptr)
        {
            c->attach(&auditObserver);
            c->attach(&progressObserver);
            for (const std::pair<std::string, double> &value : spec.startingData)
            {
                c->setData(value.first, value.second);
            }
        }
        keepOutput(capture.lines());
    }
    if (c == nullptr)
    {
        return nullptr;
    }

    std::unique_ptr<CaseRecord> record(new CaseRecord());
    record->instance = c;
    record->history.reset(new CommandHistory());
    record->invoker.reset(new Invoker(record->history.get()));
    records.push_back(std::move(record));

    log(c, "Opened with its own copy of the stages, started: Pending → Active", "Facade, Prototype, State");
    return c;
}

std::vector<WorkflowInstance *> Session::cases()
{
    std::vector<WorkflowInstance *> all;
    for (const std::unique_ptr<CaseRecord> &record : records)
    {
        all.push_back(record->instance);
    }
    return all;
}

std::vector<std::string> Session::statePath(WorkflowInstance *c)
{
    std::vector<std::string> path;
    if (c == nullptr)
    {
        return path;
    }
    path.push_back("Pending"); // every instance starts here without an event
    for (WorkflowEvent *e : c->getEventLog()->getEntries())
    {
        if (e != nullptr && e->getType() == "STATE_CHANGE")
        {
            std::string to = targetState(e->getDescription());
            if (!to.empty())
            {
                path.push_back(to);
            }
        }
    }
    return path;
}

std::vector<StageView> Session::stages(WorkflowInstance *c)
{
    std::vector<StageView> views;
    CaseRecord *record = recordFor(c);
    if (record == nullptr || c->getDefinition() == nullptr)
    {
        return views;
    }
    for (WorkItem *item : *c->getDefinition()->getStages())
    {
        Stage *stage = dynamic_cast<Stage *>(item);
        if (stage == nullptr)
        {
            continue;
        }
        StageView view;
        view.name = stage->getName();
        for (WorkItem *member : stage->getMembers())
        {
            WorkView work;
            work.name = member->getName();
            work.role = member->getRequiredPosition();
            work.severity = member->getSeverity();
            work.person = member->getResponsible() != nullptr ? member->getResponsible()->getName() : "";
            work.added = std::find(record->added.begin(), record->added.end(), member) != record->added.end();
            view.work.push_back(work);
        }
        views.push_back(view);
    }
    return views;
}

std::vector<std::string> Session::dataKeys() const
{
    return keys;
}

ActionResult Session::setData(WorkflowInstance *c, const std::string &key, double value)
{
    ActionResult result;
    if (recordFor(c) == nullptr || key.empty())
    {
        result.message = "Pick a case and a value name first.";
        return result;
    }
    double old = c->getData(key);
    c->setData(key, value);
    if (std::find(keys.begin(), keys.end(), key) == keys.end())
    {
        keys.push_back(key);
    }
    result.changed = true;
    result.message = key + ": " + number(old) + " → " + number(value);
    log(c, result.message, "Instance data");
    return result;
}

Session::CaseRecord *Session::recordFor(WorkflowInstance *c) const
{
    for (const std::unique_ptr<CaseRecord> &record : records)
    {
        if (record->instance == c && c != nullptr)
        {
            return record.get();
        }
    }
    return nullptr;
}

ActionResult Session::runCommand(WorkflowInstance *c, std::unique_ptr<Command> command, const std::string &verb)
{
    ActionResult result;
    CaseRecord *record = recordFor(c);
    if (record == nullptr)
    {
        result.message = "Open a case first.";
        return result;
    }

    State *before = c->getState();
    std::string from = before->getName();
    Memento *snapshot = c->createMemento();
    {
        CoutCapture capture;
        record->invoker->setCommand(command.get());
        record->invoker->executeCommand();
        keepOutput(capture.lines());
    }

    if (c->getState() == before)
    {
        delete snapshot;
        record->invoker->setCommand(nullptr);
        result.message = verb + " did nothing: the case is " + from + ".";
        log(c, result.message, "Command, State", true);
        return result;
    }

    record->invoker->save(snapshot); // CommandHistory keeps the snapshot (Memento caretaker)
    record->done.push_back(DoneCommand{verb, std::move(command)});
    result.changed = true;
    result.message = verb + ": " + from + " → " + c->getState()->getName();
    log(c, result.message, "Command, Memento, State");
    return result;
}

ActionResult Session::approve(WorkflowInstance *c)
{
    return runCommand(c, std::unique_ptr<Command>(new Approve(c)), "Approve");
}

ActionResult Session::reject(WorkflowInstance *c)
{
    return runCommand(c, std::unique_ptr<Command>(new Reject(c)), "Reject");
}

ActionResult Session::retry(WorkflowInstance *c)
{
    return runCommand(c, std::unique_ptr<Command>(new Retry(c)), "Retry");
}

ActionResult Session::cancel(WorkflowInstance *c)
{
    return runCommand(c, std::unique_ptr<Command>(new Cancel(c)), "Cancel");
}

ActionResult Session::undo(WorkflowInstance *c)
{
    ActionResult result;
    CaseRecord *record = recordFor(c);
    if (record == nullptr)
    {
        result.message = "Open a case first.";
        return result;
    }
    if (record->done.empty())
    {
        result.message = "Nothing to undo on this case.";
        log(c, result.message, "Command", true);
        return result;
    }

    DoneCommand last = std::move(record->done.back());
    record->done.pop_back();
    std::string from = c->getState()->getName();
    {
        CoutCapture capture;
        last.command->undo(); // restores the Memento taken before it ran
        keepOutput(capture.lines());
    }
    record->invoker->setCommand(nullptr);

    result.changed = true;
    result.message = "Undo " + last.verb + ": " + from + " → " + c->getState()->getName() + " (restored from its snapshot)";
    log(c, result.message, "Command, Memento");
    return result;
}

ActionResult Session::execute(WorkflowInstance *c)
{
    ActionResult result;
    if (recordFor(c) == nullptr)
    {
        result.message = "Open a case first.";
        return result;
    }
    State *before = c->getState();
    std::string from = before->getName();
    {
        CoutCapture capture;
        facade.executeWorkflow(c);
        keepOutput(capture.lines());
    }
    if (c->getState() == before)
    {
        result.message = "Execute did nothing: the case is " + from + ".";
        log(c, result.message, "Facade, State", true);
        return result;
    }
    result.changed = true;
    result.message = "Execute: " + from + " → " + c->getState()->getName() + " (facade, can't be undone)";
    log(c, result.message, "Facade, State");
    return result;
}

std::size_t Session::undoDepth(WorkflowInstance *c) const
{
    CaseRecord *record = recordFor(c);
    return record == nullptr ? 0 : record->done.size();
}

AssignmentStrategy *Session::makeAssignment(const std::string &kind) const
{
    std::vector<AssignmentRule> rules;
    for (const RuleSpec &r : spec.rules)
    {
        AssignmentRule rule;
        rule.conditionField = r.field;
        rule.expectedValue = r.value;
        rule.targetTitle = r.targetTitle;
        rules.push_back(rule);
    }

    const std::map<std::string, std::function<AssignmentStrategy *()>> makers = {
        {"RoleBased", [this] { return new RoleBasedAssignment(employees); }},
        {"WorkloadBased", [this] { return new WorkloadBasedAssignment(employees); }},
        {"RuleBased", [this, rules] { return new RuleBasedAssignment(rules, employees); }},
    };
    std::map<std::string, std::function<AssignmentStrategy *()>>::const_iterator make = makers.find(kind);
    return make == makers.end() ? nullptr : make->second();
}

RoutingStrategy *Session::makeRouting(const std::string &kind, WorkflowInstance *c) const
{
    WorkflowDefinition *own = c->getDefinition(); // this case's clone, so work lands in its own stages
    const RoutingSpec &r = spec.routing;

    const std::map<std::string, std::function<RoutingStrategy *()>> makers = {
        {"SeverityBased", [&]() -> RoutingStrategy * {
             if (own->findStage(r.normalStage) == nullptr || own->findStage(r.emergencyStage) == nullptr)
                 return nullptr;
             return new SeverityBasedRouting(r.normalStage, r.emergencyStage);
         }},
        {"ApprovalChain", [&]() -> RoutingStrategy * {
             Stage *standard = own->findStage(r.standardStage);
             Stage *escalation = own->findStage(r.escalationStage);
             if (standard == nullptr || escalation == nullptr || r.metricKey.empty())
                 return nullptr;
             return new ApprovalChainRouting(r.metricKey, r.approvalLimit, standard, escalation);
         }},
        {"EventBased", [&]() -> RoutingStrategy * {
             std::vector<Threshold> thresholds;
             for (const ThresholdSpec &t : r.thresholds)
             {
                 Stage *target = own->findStage(t.stage);
                 if (target == nullptr)
                     return nullptr;
                 thresholds.push_back(Threshold(t.limit, target));
             }
             if (thresholds.empty() || r.metricKey.empty())
                 return nullptr;
             return new EventBasedRouting(r.metricKey, thresholds);
         }},
    };
    std::map<std::string, std::function<RoutingStrategy *()>>::const_iterator make = makers.find(kind);
    return make == makers.end() ? nullptr : make->second();
}

SubmitResult Session::addWork(WorkflowInstance *c, const WorkSpec &work,
                              const std::string &assignment, const std::string &routing)
{
    SubmitResult result;
    CaseRecord *record = recordFor(c);
    if (record == nullptr)
    {
        result.message = "Open a case first.";
        return result;
    }
    if (work.name.empty())
    {
        result.message = "The work needs a name.";
        return result;
    }

    AssignmentStrategy *assigner = makeAssignment(assignment);
    RoutingStrategy *router = makeRouting(routing, c);
    if (assigner == nullptr || router == nullptr)
    {
        delete assigner;
        delete router;
        result.message = "\"" + work.name + "\" not added: " + routing + " routing isn't set up for this workflow.";
        log(c, result.message, "Strategy", true);
        return result;
    }

    Task *task = new Task(work.name, work.name);
    task->setRequiredPosition(work.role);
    task->setSeverity(work.severity);

    std::size_t itemsBefore = c->getWorkItems().size();
    int sentBefore = notifier.getSentCount();
    {
        CoutCapture capture;
        facade.setAssignmentStrategy(assigner); // the manager owns them from here
        facade.setRoutingStrategy(router);
        facade.submitWorkItem(c, task);
        keepOutput(capture.lines());
    }
    result.notificationsSent = notifier.getSentCount() - sentBefore;

    if (c->getWorkItems().size() == itemsBefore)
    {
        // The manager notifies once it has assigned the work and once it has
        // routed it, so no notification means nobody was found to do it.
        delete task;
        result.message = result.notificationsSent == 0
                             ? "Nobody could take \"" + work.name + "\" (" + assignment + " found no one" +
                                   (work.role.empty() ? "" : " for \"" + work.role + "\"") + ")"
                             : "\"" + work.name + "\" had someone but " + routing + " routing chose no stage";
        log(c, result.message, "Strategy, Adapter", true);
        return result;
    }

    record->added.push_back(task);
    Staff *who = task->getResponsible();
    result.accepted = true;
    result.assignee = who != nullptr ? who->getName() : "";
    result.assigneeTitle = who != nullptr ? who->getTitle() : "";
    for (const StageView &stage : stages(c))
    {
        for (const WorkView &item : stage.work)
        {
            if (item.added && item.name == work.name)
            {
                result.stage = stage.name;
            }
        }
    }
    result.message = "Added \"" + work.name + "\": " + result.assignee + " (" + result.assigneeTitle + ") in " +
                      result.stage + ", " + std::to_string(result.notificationsSent) + " notifications sent";
    log(c, result.message, "Strategy, Adapter, Composite");
    return result;
}

int Session::notificationsSent() const
{
    return notifier.getSentCount();
}

EscalationResult Session::walkChain(Staff *start, int rank) const
{
    EscalationResult result;
    result.rank = rank;
    result.startedWith = start != nullptr ? start->getName() : "";
    for (Staff *s = start; s != nullptr; s = s->getNextHandler())
    {
        ChainStep step;
        step.name = s->getName();
        step.title = s->getTitle();
        step.rank = s->getRank();
        step.canHandle = s->getRank() >= rank;
        result.steps.push_back(step);
        if (step.canHandle)
        {
            break;
        }
    }
    return result;
}

namespace
{
    /**
     * @brief "Ann ✗ → Jane ✗ → Priya ✓" for the timeline.
     * @param steps The ladder.
     * @return The sentence.
     */
    std::string ladder(const std::vector<ChainStep> &steps)
    {
        std::string text;
        for (std::size_t i = 0; i < steps.size(); ++i)
        {
            text += (i == 0 ? "" : " → ") + steps[i].name + (steps[i].canHandle ? " ✓" : " ✗");
        }
        return text;
    }
}

EscalationResult Session::escalate(WorkflowInstance *c, int rank)
{
    EscalationResult result = walkChain(chainHead, rank);
    if (chainHead == nullptr)
    {
        result.message = "There is no escalation chain set up.";
        log(c, result.message, "Chain of Responsibility", true);
        return result;
    }
    {
        CoutCapture capture;
        result.handled = chainHead->handleRequest(rank); // the engine's own verdict
        keepOutput(capture.lines());
    }
    result.message = "Rank " + std::to_string(rank) + " request: " + ladder(result.steps) +
                     (result.handled ? "" : ", nobody can approve it");
    log(c, result.message, "Chain of Responsibility", !result.handled);
    return result;
}

EscalationResult Session::escalateWork(WorkflowInstance *c, const std::string &workName, int rank)
{
    EscalationResult result;
    result.rank = rank;
    CaseRecord *record = recordFor(c);
    WorkItem *item = nullptr;
    if (record != nullptr)
    {
        for (WorkItem *added : record->added)
        {
            if (added->getName() == workName)
            {
                item = added;
            }
        }
    }
    if (item == nullptr || item->getResponsible() == nullptr)
    {
        result.message = "\"" + workName + "\" isn't work with someone responsible on this case.";
        log(c, result.message, "Chain of Responsibility", true);
        return result;
    }

    result = walkChain(item->getResponsible(), rank);
    {
        CoutCapture capture;
        result.handled = item->handleRequest(rank); // Task -> its person -> their successors
        keepOutput(capture.lines());
    }
    result.startedWith = workName + " (" + item->getResponsible()->getName() + ")";
    result.message = "\"" + workName + "\" needs rank " + std::to_string(rank) + ": " + ladder(result.steps) +
                     (result.handled ? "" : ", nobody can approve it");
    log(c, result.message, "Chain of Responsibility", !result.handled);
    return result;
}

std::string Session::progressReport(WorkflowInstance *c)
{
    if (recordFor(c) == nullptr)
    {
        return "";
    }
    ProgressReportVisitor visitor;
    {
        CoutCapture capture; // the visitor also prints as it goes; the report is what we show
        c->accept(&visitor);
    }
    log(c, "Progress report made", "Visitor");
    return visitor.getReport();
}

std::string Session::auditTrail(WorkflowInstance *c)
{
    if (recordFor(c) == nullptr)
    {
        return "";
    }
    AuditExportVisitor visitor;
    {
        CoutCapture capture;
        c->accept(&visitor);
    }
    log(c, "Audit trail exported", "Visitor");
    return visitor.getAuditTrail();
}

ActionResult Session::saveHistory(WorkflowInstance *c, const std::string &path)
{
    ActionResult result;
    if (recordFor(c) == nullptr)
    {
        result.message = "Open a case first.";
        return result;
    }
    std::size_t events = c->getEventLog()->getEntries().size();
    result.changed = WorkflowRepository::saveHistory(*c->getEventLog(), path);
    result.message = result.changed ? "Saved " + std::to_string(events) + " events to " + path
                                    : "Couldn't write " + path;
    log(c, result.message, "Repository", !result.changed);
    return result;
}

const std::vector<TimelineEntry> &Session::timeline() const
{
    return entries;
}

std::vector<TimelineEntry> Session::timeline(WorkflowInstance *c) const
{
    std::vector<TimelineEntry> mine;
    std::string id = c != nullptr ? c->getId() : "";
    for (const TimelineEntry &entry : entries)
    {
        if (entry.caseId == id)
        {
            mine.push_back(entry);
        }
    }
    return mine;
}

const std::vector<std::string> &Session::engineLog() const
{
    return output;
}

void Session::keepOutput(const std::vector<std::string> &lines)
{
    output.insert(output.end(), lines.begin(), lines.end());
    if (output.size() > kMaxOutputLines)
    {
        output.erase(output.begin(), output.begin() + (output.size() - kMaxOutputLines));
    }
}

void Session::log(WorkflowInstance *c, const std::string &text, const std::string &pattern, bool refused)
{
    TimelineEntry entry;
    entry.caseId = c != nullptr ? c->getId() : "";
    entry.time = timeNow();
    entry.text = text;
    entry.pattern = pattern;
    entry.refused = refused;
    entries.push_back(entry);
}
