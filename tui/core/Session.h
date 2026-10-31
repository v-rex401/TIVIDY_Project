/**
 * @file Session.h
 * @brief Everything the TUI does to the engine, without any drawing.
 * @author JP
 * @date 2026-10-31
 */

#ifndef SESSION_H
#define SESSION_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AuditLogObserver.h"
#include "NotificationAdapter.h"
#include "NotificationSystem.h"
#include "ProgressMonitor.h"
#include "ScenarioSetup.h"
#include "WorkflowFacade.h"

class AssignmentStrategy;
class Command;
class CommandHistory;
class Invoker;
class RoutingStrategy;
class Staff;
class WorkflowDefinition;
class WorkflowInstance;
class WorkItem;

/** @brief One line of a case's story, as shown in the timeline. */
struct TimelineEntry
{
    std::string caseId;  ///< The case it belongs to, or empty for the whole session.
    std::string time;    ///< "HH:MM:SS".
    std::string text;    ///< Plain sentence, e.g. "Approve: Pending → Active".
    std::string pattern; ///< Patterns involved, e.g. "Command, Memento, State".
    bool refused = false; ///< True when the engine said no and nothing changed.
};

/** @brief What happened when an action ran. */
struct ActionResult
{
    bool changed = false; ///< Whether anything in the engine changed.
    std::string message;  ///< The sentence that went into the timeline.
};

/** @brief The outcome of adding work to a case. */
struct SubmitResult
{
    bool accepted = false;       ///< True if someone got it and it landed in a stage.
    std::string assignee;        ///< Who got it.
    std::string assigneeTitle;   ///< Their title.
    std::string stage;           ///< Stage it was routed to.
    int notificationsSent = 0;   ///< How many notifications the adapter sent for it.
    std::string message;         ///< The sentence that went into the timeline.
};

/** @brief One rung of the escalation ladder. */
struct ChainStep
{
    std::string name;  ///< Staff member or team.
    std::string title; ///< Their title.
    int rank = 0;      ///< Their authority level.
    bool canHandle = false; ///< Whether their rank is high enough.
};

/** @brief The outcome of passing a request up the chain. */
struct EscalationResult
{
    int rank = 0;                 ///< Rank the request needed.
    std::string startedWith;      ///< What the request was given to first.
    std::vector<ChainStep> steps; ///< Who it passed through, in order, up to whoever took it.
    bool handled = false;         ///< The engine's own answer (handleRequest).
    std::string message;          ///< The sentence that went into the timeline.
};

/** @brief A work item shown under a stage. */
struct WorkView
{
    std::string name;     ///< Task name.
    std::string role;     ///< Title it needs, may be empty.
    std::string severity; ///< Severity, may be empty.
    std::string person;   ///< Who is responsible, or empty.
    bool added = false;   ///< True if it was added during this session (not part of the definition).
};

/** @brief A stage of a case and the work in it. */
struct StageView
{
    std::string name;           ///< Stage name.
    std::vector<WorkView> work; ///< Tasks in the stage.
};

/**
 * @brief The TUI's model: owns what the facade does not, and runs every action.
 *
 * Built from a ScenarioSetup, it creates the staff (Composite), links the
 * escalation chain (Chain of Responsibility), has the Director and a Builder
 * make the definition, and registers it with the WorkflowFacade. After that
 * every button in the TUI is one method here, and each one returns what
 * happened so the screens never need to know how the engine works.
 *
 * Engine output to std::cout is captured around every call and kept in
 * engineLog(), so the screen is never written to behind FTXUI's back.
 */
class Session
{
public:
    /**
     * @brief Build everything a setup describes.
     * @param setup The setup; problems() should be empty.
     */
    explicit Session(const ScenarioSetup &setup);

    /** @brief Free the staff, commands and everything else the session made. */
    ~Session();

    Session(const Session &) = delete;            ///< Not copyable: owns engine objects.
    Session &operator=(const Session &) = delete; ///< Not copyable: owns engine objects.

    // ----- setup -----

    /** @return The setup the session was built from. */
    const ScenarioSetup &setup() const;

    /** @return The definition every case is created from. */
    WorkflowDefinition *definition();

    /** @return The staff at the top of the hierarchy (teams own the rest). */
    const std::vector<Staff *> &topLevelStaff() const;

    /** @return Every employee, the pool the assignment strategies choose from. */
    const std::vector<Staff *> &roster() const;

    /**
     * @brief Add an employee while the session is running.
     * @param person The new person; their team must exist (or be empty).
     * @return What happened.
     */
    ActionResult addPerson(const PersonSpec &person);

    /** @return The escalation chain from its first member, with nobody marked as able to handle. */
    std::vector<ChainStep> chain() const;

    /**
     * @brief Text the engine prints for the staff tree (Team::printHierarchy).
     * @return One line per team or employee.
     */
    std::vector<std::string> staffTree();

    // ----- cases -----

    /**
     * @brief Open a new case through the facade (startWorkflow) and give it the starting values.
     * @return The new case.
     */
    WorkflowInstance *openCase();

    /** @return Every case opened so far, oldest first. */
    std::vector<WorkflowInstance *> cases();

    /**
     * @brief The states a case has been through, oldest first, read from its event log.
     * @param c The case.
     * @return e.g. {"Pending", "Active", "Pending", "Active"}.
     */
    std::vector<std::string> statePath(WorkflowInstance *c);

    /**
     * @brief The case's stages and the work in each.
     * @param c The case.
     * @return One entry per stage, in definition order.
     */
    std::vector<StageView> stages(WorkflowInstance *c);

    /** @return Names of the values cases carry (e.g. "triage score"). */
    std::vector<std::string> dataKeys() const;

    /**
     * @brief Change a value on one case.
     * @param c The case.
     * @param key The value's name.
     * @param value The new value.
     * @return What happened.
     */
    ActionResult setData(WorkflowInstance *c, const std::string &key, double value);

    // ----- actions on a case (Command + Memento + State) -----

    /** @brief Approve: move the case on one state. @param c The case. @return What happened. */
    ActionResult approve(WorkflowInstance *c);
    /** @brief Reject: send the case back to Pending. @param c The case. @return What happened. */
    ActionResult reject(WorkflowInstance *c);
    /** @brief Retry: reset to Pending and start again. @param c The case. @return What happened. */
    ActionResult retry(WorkflowInstance *c);
    /** @brief Cancel: stop the case. @param c The case. @return What happened. */
    ActionResult cancel(WorkflowInstance *c);
    /** @brief Undo the last action that changed the case. @param c The case. @return What happened. */
    ActionResult undo(WorkflowInstance *c);
    /** @brief Let the facade run the case on (executeWorkflow); not undoable. @param c The case. @return What happened. */
    ActionResult execute(WorkflowInstance *c);

    /**
     * @brief How many actions can be undone on a case.
     * @param c The case.
     * @return The number of commands on its undo stack.
     */
    std::size_t undoDepth(WorkflowInstance *c) const;

    // ----- work (Strategy + Adapter) -----

    /**
     * @brief Add work to a case: the assignment strategy picks a person, the routing strategy a stage.
     * @param c The case.
     * @param work What to do, the title it needs and its severity.
     * @param assignment "RoleBased", "WorkloadBased" or "RuleBased".
     * @param routing "SeverityBased", "ApprovalChain" or "EventBased".
     * @return The outcome.
     */
    SubmitResult addWork(WorkflowInstance *c, const WorkSpec &work,
                         const std::string &assignment, const std::string &routing);

    /** @return How many notifications the external system has sent. */
    int notificationsSent() const;

    // ----- escalation (Chain of Responsibility) -----

    /**
     * @brief Pass a request that needs a rank up the staff chain.
     * @param c The case it is about (for the timeline), may be null.
     * @param rank The rank the request needs.
     * @return Who it passed through and the engine's answer.
     */
    EscalationResult escalate(WorkflowInstance *c, int rank);

    /**
     * @brief Escalate a piece of added work: it starts with whoever is responsible for it.
     * @param c The case.
     * @param workName Name of the added work item.
     * @param rank The rank the request needs.
     * @return Who it passed through and the engine's answer.
     */
    EscalationResult escalateWork(WorkflowInstance *c, const std::string &workName, int rank);

    // ----- reports (Visitor) and history -----

    /** @brief Run the ProgressReportVisitor over a case. @param c The case. @return The report. */
    std::string progressReport(WorkflowInstance *c);

    /** @brief Run the AuditExportVisitor over a case. @param c The case. @return The audit trail. */
    std::string auditTrail(WorkflowInstance *c);

    /**
     * @brief Save a case's event history to CSV (WorkflowRepository).
     * @param c The case.
     * @param path Where to write.
     * @return What happened.
     */
    ActionResult saveHistory(WorkflowInstance *c, const std::string &path);

    // ----- what happened -----

    /** @return Every timeline entry, oldest first. */
    const std::vector<TimelineEntry> &timeline() const;

    /** @brief Timeline entries for one case. @param c The case. @return Its entries, oldest first. */
    std::vector<TimelineEntry> timeline(WorkflowInstance *c) const;

    /** @return Everything the engine printed, oldest first. */
    const std::vector<std::string> &engineLog() const;

private:
    /** @brief A command that changed a case, kept so it can be undone. */
    struct DoneCommand
    {
        std::string verb;                 ///< "Approve", "Reject", ...
        std::unique_ptr<Command> command; ///< Holds the snapshot taken before it ran.
    };

    /** @brief Undo stack and snapshot store for one case. */
    struct CaseRecord
    {
        WorkflowInstance *instance = nullptr;    ///< Owned by the facade.
        std::unique_ptr<CommandHistory> history; ///< Caretaker for the snapshots.
        std::unique_ptr<Invoker> invoker;        ///< Runs the commands.
        std::vector<DoneCommand> done;           ///< Commands that changed the case, newest last.
        std::vector<WorkItem *> added;           ///< Work added in this session (owned by its stage).
    };

    ScenarioSetup spec;                       ///< Copy of the setup.
    std::vector<Staff *> roots;               ///< Top of the staff tree (owned).
    std::vector<Staff *> employees;           ///< Every employee (owned through roots).
    std::map<std::string, Staff *> staffByName; ///< Every team and employee by name.
    Staff *chainHead = nullptr;               ///< First member of the escalation chain.

    NotificationSystem notifier;   ///< Adaptee: the outside notification service.
    NotificationAdapter adapter;   ///< Adapter the manager talks to.
    AuditLogObserver auditObserver;   ///< Observer attached to every case.
    ProgressMonitor progressObserver; ///< Observer attached to every case.
    WorkflowFacade facade;         ///< The only way in to the engine; declared last so it goes first.
    WorkflowDefinition *blueprint = nullptr; ///< Owned by the facade.

    std::vector<std::unique_ptr<CaseRecord>> records; ///< One per case, in opening order.
    std::vector<TimelineEntry> entries;               ///< The timeline.
    std::vector<std::string> output;                  ///< Captured engine output.
    std::vector<std::string> keys;                    ///< Names of case values.

    /** @brief Create the teams and employees and link the escalation chain. */
    void buildStaff();

    /** @brief Have the Director and the chosen Builder make the definition, and register it. */
    void buildDefinition();

    /** @brief The record for a case. @param c The case. @return Its record, or nullptr. */
    CaseRecord *recordFor(WorkflowInstance *c) const;

    /**
     * @brief Run a command through the case's Invoker and keep it if it changed anything.
     * @param c The case.
     * @param command The command, already pointing at c.
     * @param verb Its name for the timeline.
     * @return What happened.
     */
    ActionResult runCommand(WorkflowInstance *c, std::unique_ptr<Command> command, const std::string &verb);

    /** @brief Append captured engine output. @param lines The lines. */
    void keepOutput(const std::vector<std::string> &lines);

    /**
     * @brief Add a timeline entry.
     * @param c The case, or nullptr for the whole session.
     * @param text The sentence.
     * @param pattern The patterns involved.
     * @param refused Whether the engine said no.
     */
    void log(WorkflowInstance *c, const std::string &text, const std::string &pattern, bool refused = false);

    /** @brief Make an assignment strategy. @param kind Its name. @return The strategy, or nullptr if unknown. */
    AssignmentStrategy *makeAssignment(const std::string &kind) const;

    /**
     * @brief Make a routing strategy that points at one case's own stages.
     * @param kind Its name.
     * @param c The case whose cloned stages it routes to.
     * @return The strategy, or nullptr if it is unknown or a stage is missing.
     */
    RoutingStrategy *makeRouting(const std::string &kind, WorkflowInstance *c) const;

    /**
     * @brief Follow the staff chain the way handleRequest does, recording each step.
     * @param start Where the request starts.
     * @param rank The rank it needs.
     * @return The steps (handled is not filled in).
     */
    EscalationResult walkChain(Staff *start, int rank) const;
};

#endif
