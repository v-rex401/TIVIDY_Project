# TIVIDY TUI

A full-screen terminal front end for the TIVIDY workflow engine, built with FTXUI.
Every button calls the real engine: cases and work go through `WorkflowFacade`, and
commands, escalation and reports use the engine's own classes. What you see on screen
is what the engine actually did.

## Run it

```bash
make run          # or: ./build/tividy
```

- Use a terminal of at least **120 × 36**. Full screen is best for the demo.
- If stdin or stdout isn't a terminal (CI, Valgrind, a pipe), the program runs a
  **scripted text demo** of the S.H.I.E.L.D. scenario instead and exits.
  This is what `./build/tividy < /dev/null` prints.

## How it works, in one minute

1. **Pick a starting point.** There are three ready-made scenarios (Hospital,
   Insurance, S.H.I.E.L.D.), or you can choose **Build my own**.
2. Your choice becomes a `ScenarioSetup`: plain data with the people, the chain,
   the stages, the builder, the strategies and the starting values.
3. A `Session` turns that data into engine objects:
   - teams and employees (Composite)
   - the escalation chain (Chain of Responsibility)
   - the definition, made by the Director and a Builder
   - the definition is registered with the facade, and the first case is opened.
4. From then on, every key or button is **one `Session` method**. Each one calls
   the engine and returns what happened. That result goes into the case's timeline
   and the message bar.

```
 start screen ──► ScenarioSetup ──► Session ──► WorkflowFacade ──► engine
 (or builder)      (plain data)     (owns staff,     (owns definitions,
                                     commands,        instances, manager)
                                     timeline)
```

The engine prints to `std::cout` (states, observers, notifications). FTXUI draws
through `std::cout` too. So each engine call is wrapped in a `CoutCapture`, and
what was printed shows up in the **engine strip** at the bottom of the screen.

## Two ways to start

| | What you do | Good for |
|---|---|---|
| **Ready-made** | Pick Hospital, Insurance or S.H.I.E.L.D. and press enter. Steps 1 to 8 of `MainExplanation` are already done. | The demo: you go straight to running workflows. |
| **Build my own** | Four steps: **1 People**, **2 Stages**, **3 Routing**, **4 Review and start**. | Showing that nothing is hard-coded. |

Tips for **Build my own**:

- **Enter** in any field adds the person, team or stage. The cursor goes back to
  the first field, so you can type the next one straight away.
- **Tab** and **↑↓** move between fields, **←→** changes a choice, and
  **PgUp/PgDn** moves between steps.
- **Load the hospital example** fills everything in, so you can show editing
  without typing from scratch.
- The escalation chain is made for you: everyone, ordered by rank.
  Rule-based assignment gets one rule per job title, and the "Add work" quick
  picks get one task per job title.
- Step 4 lists anything that won't work, e.g. "Routing sends work to X, but
  there is no such stage".

## The screens

### 1 Cases (where the demo happens)

| Area | What it shows |
|---|---|
| **Cases** (left) | Every open case. Each one is its own `WorkflowInstance` working on its own copy of the stages. |
| **Status** | The four states drawn as a route. States the case has been in are coloured, the current one is highlighted, and edges it has used are drawn bold. The `path` line spells out every step, including going back. |
| **Stages and work** | The case's own stages and their tasks. Work you add shows with `+`, who has it, and its severity. The newest one is highlighted. |
| **Case values** | The values routing reads (e.g. `triage score`), how many actions can be undone, how many notifications were sent, and how many people are on the case. |
| **What happened** | One line per action, with the patterns involved on the right. Refused actions are red and start with ✗. |
| **Do something** (right) | Every action as a button with its key. |

### 2 Workflow

The definition the Builder made: its name, which builder made it, and the stage
links read back from the built stages. For example, Escalating gives
`Settlement → Senior Review → Assessment → Lodgement`. It also lists the stages
and tasks, and every assignment and routing setting.

### 3 People

- The staff tree, drawn by the engine's own `printHierarchy`.
- The escalation chain, with ranks.
- The ladder from the last request.
- Everyone who can be given work.
- **a** adds a person (Enter adds and keeps the form open for the next one).
- **e** tries a request.

### 4 Log

Every timeline entry across all cases, plus everything the engine printed.
**↑↓** scrolls.

## Keys

| Key | Where | Does |
|---|---|---|
| `a` | Cases | Approve: move the case on one state |
| `r` | Cases | Reject: back to Pending |
| `t` | Cases | Retry: Pending, then straight on to Active |
| `c` | Cases | Cancel |
| `u` | Cases | Undo the last approve, reject, retry or cancel |
| `x` | Cases | Execute through the facade (can't be undone) |
| `w` | Cases | Add work (Enter on a quick pick adds it right away) |
| `e` | Cases, People | Escalate a request that needs a rank |
| `v` | Cases | Change a case value |
| `p` | Cases | Reports (progress, audit trail, save history to CSV) |
| `n` | Cases | New case from the same definition |
| `a` | People | Add a person |
| `1`–`4` | anywhere | Switch tab |
| `?` | anywhere | Help: every key with its pattern |
| `s` | anywhere | Back to the start screen (switch scenario) |
| `esc` | pop-up | Close it |
| `q` | anywhere | Quit |

The buttons can also be clicked with the mouse. While a pop-up is open, single-key
shortcuts are off, so typing a name never triggers an action.

## What each action does in the engine

| Action | Engine calls | Patterns |
|---|---|---|
| Start a scenario | `new Team/Employee`, `Team::addMember`, `Staff::setNextHandler`, `WorkflowDirector::construct` with `Sequential/EscalatingWorkflowBuilder`, `WorkflowFacade::addDefinition`, `setExternalSystem(NotificationAdapter)` | Composite, Chain of Responsibility, Builder, Facade, Adapter |
| New case | `WorkflowFacade::startWorkflow` (clones the definition, Pending → Active), `attach(AuditLogObserver, ProgressMonitor)`, `setData` for the starting values | Facade, Prototype, State, Observer |
| Approve / Reject / Retry / Cancel | `Invoker::setCommand` and `executeCommand` with `Approve/Reject/Retry/Cancel`. A snapshot is kept in the case's `CommandHistory`. | Command, Memento, State, Observer |
| Undo | The last command's `undo()`, which restores its `Memento` via `setMemento` | Command, Memento |
| Execute | `WorkflowFacade::executeWorkflow` | Facade, State |
| Add work | A new `Task` with `setRequiredPosition` and `setSeverity`. Then `setAssignmentStrategy` and `setRoutingStrategy`, both built from **this case's** cloned stages, and `submitWorkItem`. The manager notifies the adapter after assigning and after routing. | Strategy, Adapter, Composite |
| Change a value | `WorkflowInstance::setData` on that case only | (case data) |
| Escalate | `Staff::handleRequest(rank)` from the head of the chain, or `Task::handleRequest(rank)`, which starts with the task's person | Chain of Responsibility |
| Reports | `WorkflowInstance::accept(ProgressReportVisitor / AuditExportVisitor)`, and `WorkflowRepository::saveHistory` | Visitor |

The TUI never decides an outcome itself:

- A refused action is spotted by checking whether the state object changed.
- A refused submit is spotted by whether the case gained a work item. The number of
  notifications sent tells you which step said no: 1 means nobody was assigned,
  2 means no stage was chosen.
- The escalation ladder is drawn by following `getNextHandler()`, but the
  yes/no shown is `handleRequest`'s own return value.

## A demo script (about 5 minutes)

1. Start screen → **Hospital admissions** → enter.
2. **Cases.** "WF-1 was opened through the facade and started: Pending → Active."
   Point at the state track.
3. **w**, then enter on *Check vitals*. Jane (Nurse) gets it, in **Assessment**,
   because the triage score is 3. Point out the notification count.
4. **v**, set the triage score to 9, then **w** and *Check vitals* again. It goes to
   **Emergency Care** now. Same task, different value: that's the Strategy pattern.
5. **w** again: in the form, switch "Where it goes" to **Severity** and add the
   *Stabilise after collapse* quick pick (Critical). It goes to Emergency Care.
6. **e**, rank 3 → enter. The ladder shows Ann ✗ → Jane ✗ → Dr Priya ✓.
   Try rank 9: nobody can take it. Then pick *"Check vitals" (Jane Mokoena first)*:
   escalation now starts with the task's person.
7. **r** (Reject: Active → Pending), **a**, **a** (→ Completed), **a** again
   (refused, shown in red), then **u u**: undo walks back through the snapshots.
   The path line shows everything.
8. **n** opens WF-2, then **c** cancels it. Switch back to WF-1 in the list: it is
   unaffected (instance isolation).
9. **p**: the progress report and audit trail come from the visitors. Save the history to CSV.
10. **2** Workflow (Builder, stage links), **3** People (Composite tree,
    chain, **a** to add someone live), **4** Log.
11. **s** → **Insurance claims**: an Escalating builder and approval-chain
    routing. Set the claim amount to 80000 and add work: it goes to Senior Review.
12. Optional: **s** → **Build my own** → *Load the hospital example* → step 4 → start.

## Code layout

```
tui/
  core/                 no FTXUI: built as tividy_tui_core, unit-tested
    ScenarioSetup.*     the setup as plain data, plus problems() to check it
    Scenarios.*         hospital, insurance and S.H.I.E.L.D. data (all industry detail lives here)
    Session.*           owns staff, commands and timeline; one method per action
    CoutCapture.*       redirects std::cout around an engine call
    TextDemo.*          the scripted run used when there is no terminal
  app/                  FTXUI screens, compiled into tividy only
    App.*               entry point: start screen, builder, main screen
    AppState.*          what the screens share (session, tab, selected case, message)
    PickerView.*        start screen
    BuilderView.*       build my own
    MainView.*          title bar, tabs, engine strip, key bar, pop-ups, shortcuts
    CasesView.*         Cases tab and the state track
    WorkflowView.* PeopleView.* LogView.*
    Forms.h, FormParts.*, Ladder.cpp, AddWorkForm.cpp, EscalateForm.cpp,
    ValueForm.cpp, ReportView.cpp, HelpView.cpp, AddPersonForm.cpp
    Theme.*             colours, panels, buttons, inputs, the choice widget
src/main.cpp            isatty? App::run() : TextDemo::run(shield)
```

- **Genericness:** nothing in `src/` or `include/` mentions a scenario.
  Hospitals, claims and S.H.I.E.L.D. only exist as strings in `tui/core/Scenarios.cpp`.
- **Ownership** (Valgrind-clean, and macOS `leaks` reports 0 for the scripted run
  and for long TUI sessions):
  - The `Session` owns the staff trees and each case's `Invoker`, `CommandHistory`
    and executed commands.
  - The facade owns the definition, the instances and their cloned definitions.
  - The manager owns the current strategies.
  - Added work is owned by the stage it was routed to.
- **Routing per case:** `ApprovalChainRouting` and `EventBasedRouting` hold
  `Stage*` pointers. Each case has its own cloned stages, so the session builds a
  fresh routing strategy from *that case's* stages for every submit. The scenario
  stores stage names, not pointers.
- **Undo:** `CommandHistory::restore()` returns the newest memento without removing
  it (a test pins this). So undo uses each command's own snapshot, newest command
  first. `CommandHistory` still keeps a snapshot of every change.

## Engine changes made for the TUI

These are small, and each one has tests. Old → new:

- **Severity could not be set.** `SeverityBasedRouting` reads `getSeverity()`, but
  nothing could set it, so every item went to the normal stage.
  Change: `WorkItem::setSeverity(const std::string&)`. It is virtual with a no-op
  default, and `Task`, `Stage` and `Handler` override it.
- **Definition had no getters.** Change: `WorkflowDefinition::getName()`,
  `getAssignmentStrategyType()`, `getRoutingStrategyType()` and `getTransitions()`.
- **The chain couldn't be read.** `getNextHandler()` was protected on `Staff` and
  `WorkItem`. Change: it is now public.
- **Clones lost their stage links.** `WorkflowDefinition::clone()` lost the links
  between stages, so every instance's stage chain stopped after one stage.
  Change: the clone now relinks the copies in the original order. Both builders'
  links survive.
- **Team members couldn't be read.** Change: `Team::getMembers()`.
- **Leak in the escalating builder.** `EscalatingWorkflowBuilder::buildDefinition()`
  leaked the old definition when called twice. Change: it frees it, as the
  sequential builder already did.
- **Participants were never filled.** `WorkflowInstance` had a `participants` list
  that nothing filled. Change: `addParticipant(Staff*)` and `getParticipants()` are
  new, and `WorkflowManager::executeWorkflow` records whoever was assigned.
- **The audit trail was always empty.** `AuditExportVisitor::getAuditTrail()`
  returned only the header and footer. Change: each visit adds a line. The console
  output is unchanged.
- **The progress report always said 0 complete.** Change:
  `ProgressReportVisitor::getReport()` lists the tasks with who has them, the
  participants and the history, and counts tasks that have someone responsible.
- **Build:** `CMakeLists.txt` adds the `tividy_tui_core` library, which
  `tividy` and `tividy_tests` link, and `tui/app` sources compile into `tividy`.
  `Doxyfile` also documents `tui/`.
- **`main.cpp`:** the hard-coded S.H.I.E.L.D. setup moved into
  `Scenarios::shield()`. `main` now starts the TUI, or the scripted run when there
  is no terminal.
- **Test fix:** a test stub in `StrategySubsystemTests.cpp` gained `override` on
  its `setSeverity`.

### UML (.vpp) updates needed

- `WorkItem`, `Task`, `Stage`, `Handler`: `+ setSeverity(severity: string) : void`
- `WorkItem`, `Staff`: `+ getNextHandler() : WorkItem*` / `Staff*`
- `WorkflowDefinition`:
  - `+ getName() : string`
  - `+ getAssignmentStrategyType() : string`
  - `+ getRoutingStrategyType() : string`
  - `+ getTransitions() : vector<pair<string,string>>`
- `Team`: `+ getMembers() : vector<Staff*>`
- `WorkflowInstance`: `+ addParticipant(staff: Staff*) : void`, `+ getParticipants() : vector<Staff*>`
- `Client`: the TUI. `Session` is its model, and it talks to the engine only
  through `WorkflowFacade` (plus the engine types it creates: staff, commands,
  strategies, visitors).

## Engine behaviour the TUI shows as it is

None of these were changed. They are listed so nobody is surprised in the demo.

- **Cancel and facade cancel.** `WorkflowFacade::cancelWorkflow` only cancels
  instances the manager has started. The TUI uses the `Cancel` command, which
  always works and can be undone.
- **Workload-based assignment** only counts work in started instances. The TUI
  opens cases with `startWorkflow`, so every case counts. It ignores job titles
  by design.
- **Severity routing** only treats `"High"` and `"Critical"` as emergencies.
- **Approval-chain routing** escalates when the value is *greater than* the limit,
  not equal to it.
- **Team requests.** `Team::handleRequest` checks the team's own rank, not its
  members'.
- **Stage handlers have nobody to ask.** Stages have no responsible person
  (builders don't use participant slots), so `Stage::handleRequest` can't find
  anyone. Escalation in the TUI therefore goes through the staff chain, or
  through a task's assigned person.
- **The visitors also print as they walk.** That output is captured and not
  shown; the report text is what the pop-up displays.

## Tests

`make test` runs everything. The TUI core has its own suites:
`SessionTest`, `ScenarioSetupTest`, `CoutCaptureTest` and `TextDemoTest`.
They cover:

- every action, and refused actions
- undo order
- all three routing strategies and all three assignment strategies
- escalation, from the chain and from a task
- reports and saving history
- that engine output never reaches the real `std::cout`
- instance isolation: changing one case leaves another untouched
- that every ready-made scenario runs end to end

The engine changes are covered by `SeverityRoutingTest`, `WorkflowDefinitionTest`,
`TeamTest`, `ParticipantTest` and new cases in `VisitorTests`.
