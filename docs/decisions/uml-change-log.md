# UML Change Log

Where the code has to differ from the class diagram (`docs/vpp/RevisedUML_2.1.vpp`). Log the change here first, then update the `.vpp`, its export and the code in the same PR (CONTRIBUTING.md rule 5).

**How to use it**

- New entry → next free ID at the bottom of its section, plus a row in the table. IDs are never reused.
- **Status:** `Open` → `Agreed` → `Done` (or `Rejected`, with a reason).
- Tick **UML** once the `.vpp` _and_ its export are updated, **Code** once the headers match.
- Only the owner (or someone they agree to) changes their classes.

```markdown
### UML-XXX · <short title>

**Why:** one line on what breaks.

| Now (UML)    | Change to          |
| ------------ | ------------------ |
| what it says | what it should say |
```

## At a glance

| ID                                                                | Change                                                       | Owner                | Status | UML | Code |
| ----------------------------------------------------------------- | ------------------------------------------------------------ | -------------------- | ------ | :-: | :--: |
| [001](#uml-001--hidden-duplicate-classes-hold-three-associations) | Reattach links held by hidden duplicate classes              | Ronin, Anke          | Open   |     |      |
| [002](#uml-002--types-that-dont-exist-in-the-model)               | Replace types that don't exist                               | Kayla, Azolile, Anke | Open   |     |      |
| [003](#uml-003--abstract-operations-arent-virtual)                | Make abstract operations virtual; add virtual destructors    | All                  | Open   |     |      |
| [004](#uml-004--abstract-types-passed-by-value)                   | Pass abstract types by pointer                               | All                  | Open   |     |      |
| [005](#uml-005--severitybasedrouting-doesnt-override)             | Rename routing operation to `selectNextStage`                | Azolile              | Open   |     |      |
| [006](#uml-006--observer-update-signatures-dont-match)            | Fix Observer `update`/`notify` signatures                    | Ronin                | Open   |     |      |
| [007](#uml-007--state-cant-tell-actions-apart)                    | Add per-action methods and `getName()` to `State`            | Jezelle              | Open   |     |      |
| [008](#uml-008--originator-cant-read-its-memento)                 | Make `WorkflowInstance` a friend of `Memento`                | Jezelle              | Open   |     |      |
| [009](#uml-009--commands-cant-reach-their-receiver)               | Protected `receiver`; `CommandHistory` owns undo             | Ronin                | Open   |     |      |
| [010](#uml-010--commandhistory-composition-is-backwards)          | Instance owns its `CommandHistory`                           | Ronin, Jezelle       | Open   |     |      |
| [011](#uml-011--workitem-interface-is-incomplete)                 | Add `accept`/`clone`/`getStatus` to `WorkItem`; drop Handler | Kayla                | Open   |     |      |
| [012](#uml-012--staff-has-no-common-operations)                   | Give `Staff` a shared interface                              | Jezelle              | Open   |     |      |
| [013](#uml-013--workflowdefinition-cant-store-builder-output)     | Add fields to `WorkflowDefinition`                           | Kayla                | Open   |     |      |
| [014](#uml-014--builder-cant-return-its-product)                  | Move `getWorkflowDefinition()` to `WorkflowBuilder`          | Kayla                | Open   |     |      |
| [015](#uml-015--visitor-double-dispatch-is-incomplete)            | Add `accept` to `Task`, `Staff`, `WorkflowEvent`             | Vashti               | Open   |     |      |
| [016](#uml-016--adapter-operations-carry-no-data)                 | Give Adapter operations real data                            | Anke                 | Open   |     |      |
| [017](#uml-017--workflowmanager-stores-nothing)                   | Manager owns instances and definitions                       | Azolile              | Open   |     |      |
| [018](#uml-018--workflowfacade-is-untyped-and-incomplete)         | Type the facade; add TUI operations                          | JP                   | Open   | ✅  |      |
| [019](#uml-019--client-has-no-operations)                         | Give `Client` (TUI) operations                               | JP                   | Open   |     |      |
| [020](#uml-020--typos-in-names)                                   | Fix typos in class and operation names                       | Ronin, Azolile, Anke | Open   |     |      |
| [021](#uml-021--no-class-diagram-export)                          | Export the class diagram to `docs/images/`                   | Team                 | Open   |     |      |

All entries logged 2026-10-22 by JP.

---

## Whole model

### UML-001 · Hidden duplicate classes hold three associations

**Why:** the links look connected on the diagram, but the generated code has none of them.

| Now (UML)                                    | Change to                                  |
| -------------------------------------------- | ------------------------------------------ |
| `Invoker → Command3`                         | `Invoker → Command`                        |
| `CommandHistory → Memento3`                  | `CommandHistory → Memento`                 |
| `NotificationAdapter2 → NotificationSystem2` | `NotificationAdapter → NotificationSystem` |
| 4 hidden duplicate classes in the model      | Delete them (Model Explorer → delete)      |

### UML-002 · Types that don't exist in the model

**Why:** the headers can't compile.

| Now (UML)                                    | Change to                                     |
| -------------------------------------------- | --------------------------------------------- |
| `WorkItem2`, `WorkflowItem`                  | `WorkItem`                                    |
| `List<…>`                                    | `std::vector<…*>`                             |
| `String`                                     | `string`                                      |
| `Threshold`, `AssignmentRule` (not in model) | Add them (or `Threshold` → `map<string,int>`) |
| `construct(…, tasks: map<string, vector>)`   | `map<string, vector<string>>`                 |
| `Handler::addMember/removeMember(Component)` | Give the parameter a type                     |

### UML-003 · Abstract operations aren't virtual

**Why:** base-pointer calls don't dispatch, and `delete basePtr` is undefined (fails Valgrind).

| Now (UML)                                                               | Change to         |
| ----------------------------------------------------------------------- | ----------------- |
| `visit*`, `Observer::update`, `ExternalSystem::request`, builder steps  | `virtual … = 0`   |
| Strategy, `WorkItem`, `Staff`, `WorkflowVisitor` bases not `{abstract}` | Mark `{abstract}` |
| No destructor on any abstract base                                      | `virtual ~X() {}` |

### UML-004 · Abstract types passed by value

**Why:** abstract types can't be passed by value, and the rest get sliced.

| Now (UML)                                                                 | Change to                            |
| ------------------------------------------------------------------------- | ------------------------------------ |
| `WorkItem`, `State`, `Memento`, `EventLog`, `Staff`, `WorkflowDefinition` | Pointers (`WorkItem*`, …)            |
| `getWorkItem() : void`                                                    | `getWorkItems() : vector<WorkItem*>` |

### UML-020 · Typos in names

**Why:** renaming later means changing a shared interface.

| Now (UML)         | Change to         |
| ----------------- | ----------------- |
| `ProgessMonitor`  | `ProgressMonitor` |
| `thereshold`      | `threshold`       |
| `WorklflowEvent`  | `WorkflowEvent`   |
| `WorkFlowSubject` | `WorkflowSubject` |

### UML-021 · No class-diagram export

**Why:** CONTRIBUTING.md and the report need it.

| Now (UML)                                         | Change to                                           |
| ------------------------------------------------- | --------------------------------------------------- |
| `docs/images/` has only activity + state diagrams | Export the class diagram; re-export on every change |

## Command, Observer and Memento (Ronin, Jezelle)

### UML-006 · Observer `update` signatures don't match

**Why:** the concrete observers don't override `update`, and `notify()` has no event to send.

| Now (UML)                                             | Change to                                   |
| ----------------------------------------------------- | ------------------------------------------- |
| Concrete observers: `update()`                        | `update(WorkflowEvent*, WorkflowInstance*)` |
| `WorkFlowSubject::notify()`                           | `notify(WorkflowEvent*)`                    |
| `attach/detach/notify` abstract on the subject        | Implemented in `WorkFlowSubject`            |
| `WorkflowInstance` re-declares `attach/detach/notify` | Remove                                      |

### UML-008 · Originator can't read its memento

**Why:** `WorkflowInstance` can't restore from its own snapshot.

| Now (UML)                                  | Change to                                               |
| ------------------------------------------ | ------------------------------------------------------- |
| `# getState()` / `# setState()`, no friend | Private; «friend» `WorkflowInstance → Memento`          |
| Stores only shared `State*` / `WorkItem*`  | Agree snapshot contents (state, current item, statuses) |

### UML-009 · Commands can't reach their receiver

**Why:** concrete commands can't call the instance, and it's unclear who owns undo.

| Now (UML)                          | Change to                                       |
| ---------------------------------- | ----------------------------------------------- |
| `- receiver`, no constructor       | `# receiver`, `Command(WorkflowInstance*)`      |
| `Invoker::save(m)` / `undo(m)`     | `undo()`; `executeCommand()` saves first        |
| No `Invoker → CommandHistory` link | Add association; `CommandHistory` owns mementos |

### UML-010 · CommandHistory composition is backwards

**Why:** wrong ownership means double deletes or leaks.

| Now (UML)                       | Change to                         |
| ------------------------------- | --------------------------------- |
| Diamond on `CommandHistory` end | Diamond on `WorkflowInstance` end |

## State and Staff (Jezelle)

### UML-007 · State can't tell actions apart

**Why:** invalid actions (e.g. cancelling a completed instance) can't be rejected, and the TUI can't show the state.

| Now (UML)          | Change to                                                              |
| ------------------ | ---------------------------------------------------------------------- |
| Only `handle(ctx)` | `approve/reject/retry/cancel(ctx)` — invalid returns `false` or throws |
| No name            | `getName() : string`                                                   |

### UML-012 · Staff has no common operations

**Why:** clients can't treat a `Staff*` uniformly, and deleting one cascades.

| Now (UML)                            | Change to                                                            |
| ------------------------------------ | -------------------------------------------------------------------- |
| Getters only on `Employee` / `Team`  | Abstract `getName/getTitle/getRank/printHierarchy/accept` on `Staff` |
| `Team::operation()`, no `add/remove` | `add(Staff*)`, `remove(Staff*)`                                      |
| `successor` is a composition         | Plain association, or remove                                         |

## Builder and WorkItem (Kayla)

### UML-011 · WorkItem interface is incomplete

**Why:** you can't visit or clone through a `WorkItem*`, and `Handler` overlaps `Stage`.

| Now (UML)                                      | Change to                                               |
| ---------------------------------------------- | ------------------------------------------------------- |
| No `accept` / `clone` on `WorkItem` or `Task`  | Abstract `accept`, `clone() : WorkItem*`, `getStatus()` |
| `Handler` duplicates `successor`, Staff fields | Remove `Handler`, or document its role                  |

### UML-013 · WorkflowDefinition can't store builder output

**Why:** the builder steps have nowhere to put their results.

| Now (UML)                      | Change to                                                                  |
| ------------------------------ | -------------------------------------------------------------------------- |
| Only `name`, `stages`          | Add transitions, participant slots, strategy types, initial data + getters |
| No `getName()`                 | Add `getName()`                                                            |
| `setBuilder(WorkflowBuilder*)` | Remove                                                                     |

### UML-014 · Builder can't return its product

**Why:** the client only holds a `WorkflowBuilder*`.

| Now (UML)                                            | Change to                    |
| ---------------------------------------------------- | ---------------------------- |
| `definition`, `getWorkflowDefinition()` on concretes | Move up to `WorkflowBuilder` |
| `WorkflowDirector` constructor returns `void`        | No return type               |

## Strategy, Manager and Adapter (Azolile, Anke)

### UML-005 · SeverityBasedRouting doesn't override

**Why:** `SeverityBasedRouting` stays abstract and can't be created.

| Now (UML)                                              | Change to                                                            |
| ------------------------------------------------------ | -------------------------------------------------------------------- |
| Base `selectItemState()`, concrete `selectItemStage()` | `selectNextStage(WorkItem*, WorkflowInstance*) : Stage*` in all four |

### UML-016 · Adapter operations carry no data

**Why:** there's no message to deliver and nothing to adapt.

| Now (UML)                         | Change to                                    |
| --------------------------------- | -------------------------------------------- |
| `ExternalSystem::request()`       | `request(WorkflowEvent*)`                    |
| `sendNotification()`              | `sendNotification(recipient, subject, body)` |
| Manager `..> ExternalSystem` only | `WorkflowManager.externalSystem` + setter    |

### UML-017 · WorkflowManager stores nothing

**Why:** instances leak, and the TUI can't find one.

| Now (UML)                        | Change to                                                                  |
| -------------------------------- | -------------------------------------------------------------------------- |
| No storage                       | Owns `instances`, `definitions`; destructor                                |
| No lookups                       | `getInstance(id)`, `getInstances()`, `addDefinition()`, `getDefinitions()` |
| `createInstance` returns nothing | Returns `WorkflowInstance*`                                                |

## Visitor (Vashti)

### UML-015 · Visitor double dispatch is incomplete

**Why:** tasks, participants and events can never be visited.

| Now (UML)                                      | Change to                                         |
| ---------------------------------------------- | ------------------------------------------------- |
| `accept` only on `Stage`, definition, instance | Add to `Task`, `Staff` (UML-012), `WorkflowEvent` |
| `visitWorkItem`                                | Remove, or keep as documented fallback            |
| `visitTask(Task: Task*)`                       | `visitTask(t: Task*)`                             |

## Facade and TUI (JP)

### UML-018 · WorkflowFacade is untyped and incomplete

**Why:** headers get placeholder `int`s, and nothing serves the TUI.

| Now (UML)                                  | Change to                                                               |
| ------------------------------------------ | ----------------------------------------------------------------------- |
| Untyped parameters, all `void`             | Typed parameters; return the instance                                   |
| `startWorkflow` + `createWorkflowInstance` | Merge into one                                                          |
| No TUI operations                          | List definitions/instances, current work, action, undo, history, report |

Signatures to be agreed with Azolile (UML-017) and Ronin (UML-009).

### UML-019 · Client has no operations

**Why:** the TUI is graded, and the diagram shows nothing of it.

| Now (UML)                  | Change to                                            |
| -------------------------- | ---------------------------------------------------- |
| Empty class                | `facade`, `run()`, menu operations, input helpers    |
| No Command / Invoker links | `Client ..> Command «creates»`, `Client ..> Invoker` |
