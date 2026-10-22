# UML Change Log

Differences between the class diagram (`docs/vpp/RevisedUML_2.1.vpp`) and code that will work. The code must reflect the UML. When it can't, the change is logged here first, and the UML is updated in the same PR as the code (CONTRIBUTING.md rule 5).

## How to use this log

- Add an entry whenever the code has to differ from the UML, or the UML can't be implemented as drawn.
- IDs never change or get reused (`UML-001`, `UML-002`, …). Add new entries at the bottom.
- **Status:** `Open` (found, not discussed) → `Agreed` (owner/team accepted the change) → `Done` (both the `.vpp` and the code are updated) or `Rejected` (with a reason).
- Tick **UML updated** only once the `.vpp` has been edited *and* the diagram export has been re-generated.
- Owner = subsystem owner from the task split in the Project Checklist. Only the owner (or someone they agree to) changes their classes.

### Entry template

```markdown
### UML-XXX: <short title>
| Field | Value |
|---|---|
| Status | Open |
| Classes | <classes> |
| Pattern | <pattern / none> |
| Owner | <name> |
| Logged | YYYY-MM-DD by <name> |

**UML currently says:** …
**Problem:** …
**Proposed change:** …

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated
```

## Summary

| ID | Title | Owner | Status |
|---|---|---|---|
| UML-001 | Hidden duplicate classes hold three associations | Ronin, Anke | Open |
| UML-002 | Types that don't exist in the model | Kayla, Azolile, Anke | Open |
| UML-003 | Abstract operations not virtual; no virtual destructors | All | Open |
| UML-004 | Abstract types passed or returned by value | Azolile, Jezelle, Kayla, Ronin | Open |
| UML-005 | `SeverityBasedRouting` doesn't override the strategy operation | Azolile | Open |
| UML-006 | Observer `update` signatures don't match | Ronin | Open |
| UML-007 | `State` has no per-action transitions or name | Jezelle | Open |
| UML-008 | Memento getters are protected and nobody is a friend | Jezelle | Open |
| UML-009 | `Command::receiver` is private; Invoker/CommandHistory responsibilities overlap | Ronin | Open |
| UML-010 | CommandHistory–WorkflowInstance composition is backwards | Ronin, Jezelle | Open |
| UML-011 | `WorkItem` composite/chain interface is incomplete; `Handler` duplicates it | Kayla | Open |
| UML-012 | `Staff` base has no common operations | Jezelle | Open |
| UML-013 | `WorkflowDefinition` has nowhere to store what the builder sets | Kayla | Open |
| UML-014 | `WorkflowBuilder` can't return its product polymorphically | Kayla | Open |
| UML-015 | Visitor double dispatch is incomplete | Vashti, Kayla | Open |
| UML-016 | Adapter operations carry no data; Manager has no `ExternalSystem*` | Anke, Azolile | Open |
| UML-017 | `WorkflowManager` keeps no registry of instances or definitions | Azolile | Open |
| UML-018 | `WorkflowFacade` has untyped parameters and is missing TUI operations | JP | Open |
| UML-019 | `Client` (TUI) has no operations | JP | Open |
| UML-020 | Typos in class and operation names | Various | Open |
| UML-021 | No class-diagram export in `docs/images/` | Team | Open |

---

### UML-001: Hidden duplicate classes hold three associations
| Field | Value |
|---|---|
| Status | Open |
| Classes | `Command3`, `Memento3`, `NotificationAdapter2`, `NotificationSystem2` |
| Pattern | Command, Memento, Adapter |
| Owner | Ronin (Command, Memento link), Anke (Adapter) |
| Logged | 2026-10-22 by JP |

**UML currently says:** The model has four classes that appear on no diagram. Three associations are attached to them instead of to the real classes: `Invoker → Command3`, `CommandHistory → Memento3` and `NotificationAdapter2 → NotificationSystem2`.
**Problem:** On the diagram these links appear to connect the real classes, but in the model they don't. As a result, the generated code has no `vector<Memento*>`-style link from Invoker to Command, from CommandHistory to Memento, or from Adapter to Adaptee. `Memento3` also uses a nonexistent `WorkflowState` type.
**Proposed change:** Reconnect each association to the real class (`Command`, `Memento`, `NotificationSystem`), then delete the four duplicates from the model (Model Explorer → delete, not just "delete from diagram").

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-002: Types that don't exist in the model
| Field | Value |
|---|---|
| Status | Open |
| Classes | `Stage`, `EventBasedRouting`, `RuleBasedAssignment`, `WorkflowDirector`, `EventLog`, several |
| Pattern | — |
| Owner | Kayla, Azolile, Anke |
| Logged | 2026-10-22 by JP |

**UML currently says:**
- `Stage` uses `WorkItem2*` (members, `addMember`, `removeMember`, `setNextHandler`), and `clone()` returns `WorkflowItem`.
- `EventBasedRouting.thresholds : List<Threshold>` and `RuleBasedAssignment.rules : List<AssignmentRule>`. Neither `Threshold` nor `AssignmentRule` exists in the model, even though the task split assigns `AssignmentRule.h` to Azolile.
- `WorkflowDirector::construct(..., tasks: map<string, vector>)` has no element type for the `vector`.
- `String` (Java) is used in `WorkItem`, `Task`, `Handler`, `Employee` and `Team`, alongside `string` elsewhere.
- `Handler::addMember/removeMember(Component)` have no parameter type.

**Problem:** The generated headers can't compile.
**Proposed change:**
- `WorkItem2`/`WorkflowItem` → `WorkItem`.
- `List<…>` → `std::vector<…*>`.
- Add `AssignmentRule` (and `Threshold`, or fold it into a `std::map<std::string,int>`) to the model.
- Use `map<string, vector<string>>`.
- Use `string` everywhere.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-003: Abstract operations not virtual; no virtual destructors
| Field | Value |
|---|---|
| Status | Open |
| Classes | `WorkflowVisitor`, `Observer`, `ExternalSystem`, `WorkflowBuilder`, `WorkItem`, `Staff`, `AssignmentStrategy`, `RoutingStrategy`, `WorkFlowSubject` |
| Pattern | All |
| Owner | Each base-class owner |
| Logged | 2026-10-22 by JP |

**UML currently says:** These operations are *not* marked abstract: all of `WorkflowVisitor`'s `visit*`, `Observer::update`, `ExternalSystem::request`, and `WorkflowBuilder::buildTransition/buildParticipantSlot/set*/buildDefinition`. No abstract base has a destructor. `AssignmentStrategy`, `RoutingStrategy`, `WorkItem`, `Staff` and `WorkflowVisitor` aren't flagged `{abstract}`.
**Problem:** Without `virtual`, calls through a base pointer don't dispatch, which breaks every pattern that depends on polymorphism. Without a virtual destructor, `delete basePtr` is undefined behaviour, and the Valgrind CI check will fail.
**Proposed change:** Mark these classes `{abstract}` and their operations abstract (italic in VP). Add a public virtual destructor `~X()` to every abstract base. In code, declare them `virtual ... = 0;` and `virtual ~X() {}`.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-004: Abstract types passed or returned by value
| Field | Value |
|---|---|
| Status | Open |
| Classes | `RoutingStrategy` (+3), `WorkflowManager`, `WorkflowInstance`, `Task`, `CommandHistory`, builders |
| Pattern | Strategy, State, Memento, Builder |
| Owner | Azolile, Jezelle, Kayla, Ronin |
| Logged | 2026-10-22 by JP |

**UML currently says:**
- `selectItemState(item: WorkItem, …)` and `routeWorkItem(wi: WorkItem, …)` take the item by value.
- `WorkflowInstance::getState() : State`, `createMemento() : Memento`, `getEventLog() : EventLog` and `getWorkItem() : void` return by value or return nothing.
- `Task.responsible : Staff` is stored by value.
- `CommandHistory::restore() : Memento` and `getWorkflowDefinition() : WorkflowDefinition` return by value.

**Problem:** An abstract class can't be passed or returned by value, so this doesn't compile. A polymorphic object stored by value gets sliced. `getWorkItem()` returns nothing.
**Proposed change:** Use pointers throughout: `WorkItem*`, `State*`, `Memento*`, `EventLog*`, `Staff*`, `WorkflowDefinition*`. Change `getWorkItem()` to `std::vector<WorkItem*> getWorkItems()`.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-005: `SeverityBasedRouting` doesn't override the strategy operation
| Field | Value |
|---|---|
| Status | Open |
| Classes | `RoutingStrategy`, `SeverityBasedRouting`, `ApprovalChainRouting`, `EventBasedRouting` |
| Pattern | Strategy |
| Owner | Azolile |
| Logged | 2026-10-22 by JP |

**UML currently says:** The base declares `selectItemState(...) : Stage`, but `SeverityBasedRouting` declares `selectItemStage(...)`.
**Problem:** It doesn't override the base operation, so `SeverityBasedRouting` stays abstract and can't be instantiated. The base name `selectItemState` also says "State" but returns a `Stage`.
**Proposed change:** Rename the operation to `selectNextStage(WorkItem*, WorkflowInstance*) : Stage*` in all four classes.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-006: Observer `update` signatures don't match
| Field | Value |
|---|---|
| Status | Open |
| Classes | `Observer`, `AuditLogObserver`, `ProgessMonitor`, `WorkFlowSubject`, `WorkflowInstance` |
| Pattern | Observer |
| Owner | Ronin |
| Logged | 2026-10-22 by JP |

**UML currently says:** `Observer::update(event: WorkflowEvent*, context: WorkflowInstance*)`, but both concrete observers declare `update()` with no parameters. `WorkFlowSubject::notify()` takes no event. `WorkflowInstance` re-declares `attach/detach/notify`, and the subject owns `observerList` while leaving those operations abstract.
**Problem:** The concrete observers don't override `update`. `notify()` has no event to pass on.
**Proposed change:** Make `update(WorkflowEvent*, WorkflowInstance*)` abstract on `Observer` and override it in both concretes. Implement `attach/detach/notify(WorkflowEvent*)` concretely in `WorkFlowSubject` (it already has the list), and remove the re-declarations from `WorkflowInstance`.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-007: `State` has no per-action transitions or name
| Field | Value |
|---|---|
| Status | Open |
| Classes | `State`, `Pending`, `Active`, `Completed`, `Cancelled`, Commands |
| Pattern | State, Command |
| Owner | Jezelle (coordinate with Ronin) |
| Logged | 2026-10-22 by JP |

**UML currently says:** `State` only has `handle(ctx)`.
**Problem:** Approve, reject, retry and cancel each need a different, state-dependent result. For example, cancelling a `Completed` instance must be rejected as invalid, and the spec requires "error handling for invalid transitions". A single `handle()` can't tell the actions apart. The TUI also has no way to display the current state.
**Proposed change:** Add `approve(ctx)`, `reject(ctx)`, `retry(ctx)`, `cancel(ctx)` and `getName() : string` to `State`. Each command calls the matching method on the receiver's current state. An invalid action returns `false` or throws a documented exception.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-008: Memento getters are protected and nobody is a friend
| Field | Value |
|---|---|
| Status | Open |
| Classes | `Memento`, `WorkflowInstance` |
| Pattern | Memento |
| Owner | Jezelle |
| Logged | 2026-10-22 by JP |

**UML currently says:** `# getState()` / `# setState()`. No class is a friend and none derives from `Memento`.
**Problem:** The originator (`WorkflowInstance`) can't read its own memento. The memento also only stores `State*` and `WorkItem*` (shared pointers), so it doesn't capture the instance's data or history position.
**Proposed change:** Add a «friend» dependency `WorkflowInstance → Memento` and make the constructor and getters private. Decide exactly what a snapshot contains: state, current work item, and per-item status.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-009: `Command::receiver` is private; Invoker/CommandHistory responsibilities overlap
| Field | Value |
|---|---|
| Status | Open |
| Classes | `Command` (+4), `Invoker`, `CommandHistory` |
| Pattern | Command, Memento |
| Owner | Ronin |
| Logged | 2026-10-22 by JP |

**UML currently says:** `- receiver` is private, and there's no constructor to set it. `Invoker` has `save(m)` and `undo(m)` but no link to `CommandHistory`, which also has `save/restore`.
**Problem:** Concrete commands can't reach the receiver. It's unclear who owns the undo stack.
**Proposed change:** Make `receiver` protected (`#`) and add `Command(WorkflowInstance*)`. Give `Invoker` an association to `CommandHistory`, and let `CommandHistory` own the mementos (`save/restore`). `Invoker::executeCommand()` saves a snapshot first, and `Invoker::undo()` takes no parameter.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-010: CommandHistory–WorkflowInstance composition is backwards
| Field | Value |
|---|---|
| Status | Open |
| Classes | `CommandHistory`, `WorkflowInstance` |
| Pattern | Memento |
| Owner | Ronin, Jezelle |
| Logged | 2026-10-22 by JP |

**UML currently says:** The composition diamond is on the `CommandHistory` end, meaning the history owns the instance. Meanwhile `WorkflowInstance.history : CommandHistory*`.
**Problem:** Ownership is inverted, which leads to double deletes or leaks under Valgrind.
**Proposed change:** Move the diamond to the `WorkflowInstance` end: each instance owns its own history, which also supports instance isolation.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-011: `WorkItem` composite/chain interface is incomplete; `Handler` duplicates it
| Field | Value |
|---|---|
| Status | Open |
| Classes | `WorkItem`, `Stage`, `Task`, `Handler` |
| Pattern | Composite, Chain of Responsibility, Prototype, Visitor |
| Owner | Kayla |
| Logged | 2026-10-22 by JP |

**UML currently says:**
- `WorkItem` has no `accept()` (despite the `WorkItem ..> WorkflowVisitor «accept»` dependency) and no `clone()`, even though the task split calls for Prototype.
- `Stage` has both. `Task` has neither.
- `Handler` (also a `WorkItem`) repeats `successor : Handler*`, has a stray `setNextHandler2`, holds `members : vector<WorkItem>` by value, and copies Staff-like fields (`name`, `title`, `responsible`).

**Problem:** You can't visit or clone through a `WorkItem*`. `Handler`'s role overlaps with `Stage` (composite) and `WorkItem` (chain).
**Proposed change:** Add abstract `accept(WorkflowVisitor*)`, `clone() : WorkItem*` and `getStatus()` to `WorkItem`, and implement them in `Task` and `Stage`. Either remove `Handler` (Stage is the composite, and `WorkItem.successor` already gives the chain) or document its distinct role.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-012: `Staff` base has no common operations
| Field | Value |
|---|---|
| Status | Open |
| Classes | `Staff`, `Employee`, `Team` |
| Pattern | Composite |
| Owner | Jezelle |
| Logged | 2026-10-22 by JP |

**UML currently says:** `Staff` only has `successor : Staff*`, a self-composition. `Employee` and `Team` each declare `getName/getTitle/getRank` independently. `Team` has a leftover `operation()` and no `add/remove`.
**Problem:** Clients can't treat a `Staff*` uniformly, which is the point of Composite. The composition on `successor` means each staff member owns the next one, so deletion would cascade.
**Proposed change:** Add abstract `getName()`, `getTitle()`/`getRole()`, `getRank()`, `printHierarchy(string)` and `accept(WorkflowVisitor*)` to `Staff`. Add `add(Staff*)`/`remove(Staff*)` to `Team`. Make `successor` a plain association (no ownership), or remove it if it isn't used for escalation.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-013: `WorkflowDefinition` has nowhere to store what the builder sets
| Field | Value |
|---|---|
| Status | Open |
| Classes | `WorkflowDefinition`, builders |
| Pattern | Builder, Prototype |
| Owner | Kayla |
| Logged | 2026-10-22 by JP |

**UML currently says:** The builders have `buildTransition`, `buildParticipantSlot`, `setAssignmentStrategyType`, `setRoutingStrategyType` and `setInitialData`. `WorkflowDefinition` only stores `name` and `stages`. It also has `setBuilder(WorkflowBuilder*)`.
**Problem:** The builder steps have nowhere to put their results. A product that knows its builder reverses the Builder pattern.
**Proposed change:** Add the matching fields to `WorkflowDefinition`: transitions, participant slots (role per stage), strategy type names and initial data, with getters. Remove `setBuilder`. Add `getName()`.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-014: `WorkflowBuilder` can't return its product polymorphically
| Field | Value |
|---|---|
| Status | Open |
| Classes | `WorkflowBuilder`, `SequentialWorkflowBuilder`, `EscalatingWorkflowBuilder`, `WorkflowDirector` |
| Pattern | Builder |
| Owner | Kayla |
| Logged | 2026-10-22 by JP |

**UML currently says:** `getWorkflowDefinition()` and the `definition` field exist only on the concrete builders. The `WorkflowDirector` constructor is modelled with a `void` return.
**Problem:** The client only holds a `WorkflowBuilder*`, so it can't get the result.
**Proposed change:** Move `definition` (protected) and `getWorkflowDefinition() : WorkflowDefinition*` up to `WorkflowBuilder`. Model the constructor without a return type.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-015: Visitor double dispatch is incomplete
| Field | Value |
|---|---|
| Status | Open |
| Classes | `WorkflowVisitor` (+2), `Task`, `Staff`, `WorkflowEvent`, `WorkflowDefinition` |
| Pattern | Visitor |
| Owner | Vashti (coordinate with Kayla, Jezelle, Anke) |
| Logged | 2026-10-22 by JP |

**UML currently says:** The visitor has `visitStage`, `visitTask`, `visitWorkItem`, `visitParticipant(Staff*)` and `visitHistoryEntry(WorkflowEvent*)`, but only `Stage`, `WorkflowDefinition` and `WorkflowInstance` have `accept`. `visitTask(Task: Task*)` names its parameter the same as its type.
**Problem:** Tasks, participants and events can never be visited. `visitWorkItem` is never reached when the concrete types have their own visit methods.
**Proposed change:** Add `accept(WorkflowVisitor*)` to `Task`, `Staff` (via UML-012) and `WorkflowEvent`. Remove `visitWorkItem`, or keep it only as a documented fallback. Rename the parameter to `t`.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-016: Adapter operations carry no data; Manager has no `ExternalSystem*`
| Field | Value |
|---|---|
| Status | Open |
| Classes | `ExternalSystem`, `NotificationAdapter`, `NotificationSystem`, `WorkflowManager` |
| Pattern | Adapter |
| Owner | Anke (coordinate with Azolile) |
| Logged | 2026-10-22 by JP |

**UML currently says:** `ExternalSystem::request()` and `NotificationSystem::sendNotification()` take no parameters. `WorkflowManager ..> ExternalSystem «uses»` is only a dependency.
**Problem:** There's no message to deliver, and the interfaces aren't actually incompatible, which is what justifies an Adapter. The manager can't hold the adapter, so it can't be "wired through WorkflowManager" as the task split says.
**Proposed change:** `request(event: WorkflowEvent*)`, and the adaptee gets its own differently-shaped call (for example `sendNotification(recipient: string, subject: string, body: string)`). Add `WorkflowManager.externalSystem : ExternalSystem*` plus a setter.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-017: `WorkflowManager` keeps no registry of instances or definitions
| Field | Value |
|---|---|
| Status | Open |
| Classes | `WorkflowManager` |
| Pattern | Facade (subsystem) |
| Owner | Azolile |
| Logged | 2026-10-22 by JP |

**UML currently says:** The manager creates instances (`createInstance`) but stores nothing.
**Problem:** Nothing owns the instances, so they leak, and nothing can find "instance 3" for the TUI. Demonstrating instance isolation needs several live instances.
**Proposed change:** Add `instances : vector<WorkflowInstance*>` (composition) and `definitions : vector<WorkflowDefinition*>`. Add `getInstance(id: string) : WorkflowInstance*`, `getInstances()`, `addDefinition(WorkflowDefinition*)`, `getDefinitions()` and `~WorkflowManager()`. Make `createInstance` return `WorkflowInstance*`.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-018: `WorkflowFacade` has untyped parameters and is missing TUI operations
| Field | Value |
|---|---|
| Status | Open |
| Classes | `WorkflowFacade` |
| Pattern | Facade |
| Owner | JP |
| Logged | 2026-10-22 by JP |

**UML currently says:** `startWorkflow(definition)`, `createWorkflowInstance(definition)` and `submitWorkItem(instance, workitem)` have no types. Everything returns `void`. `startWorkflow` and `createWorkflowInstance` overlap.
**Problem:** The headers generate placeholder `int`s. The demo also requires the TUI to show available work, take actions, show history and run reports, and none of that is on the facade.
**Proposed change:** Type the parameters and return the instance. Merge the overlapping start/create operations. Add operations for the TUI: list definitions and instances, get current work, perform an action, undo, get history and get a report. The exact signatures will be agreed with Azolile (UML-017) and Ronin (UML-009).

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-019: `Client` (TUI) has no operations
| Field | Value |
|---|---|
| Status | Open |
| Classes | `Client` |
| Pattern | Facade / Builder / Command client |
| Owner | JP |
| Logged | 2026-10-22 by JP |

**UML currently says:** `Client` is an empty class with dependencies on `WorkflowFacade`, `WorkflowDirector` and `WorkflowBuilder`.
**Problem:** The TUI is a graded deliverable, and the class diagram shows nothing of it. It also creates Commands and passes them to the `Invoker`, but those dependencies aren't drawn.
**Proposed change:** Add `facade : WorkflowFacade*`, `run()`, the menu operations and input helpers. Add `Client ..> Command «creates»` and `Client ..> Invoker`.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-020: Typos in class and operation names
| Field | Value |
|---|---|
| Status | Open |
| Classes | `ProgessMonitor`, `ApprovalChainRouting`, `EventLog`, `WorkFlowSubject` |
| Pattern | — |
| Owner | Ronin, Azolile, Anke |
| Logged | 2026-10-22 by JP |

**UML currently says:**
- `ProgessMonitor` → should be `ProgressMonitor` (the task split already spells it correctly).
- `ApprovalChainRouting.thereshold` → `threshold`.
- `EventLog::getEntries() : vector<WorklflowEvent*>` → `WorkflowEvent*` (already on the General Checklist).
- `WorkFlowSubject` → `WorkflowSubject`, to match `WorkflowInstance` and the other `Workflow*` classes.

**Problem:** Renaming later means changing a shared interface, so it's cheaper to do now, while the headers are being written.
**Proposed change:** Rename in the `.vpp`, then rename the files and headers.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated

### UML-021: No class-diagram export in `docs/images/`
| Field | Value |
|---|---|
| Status | Open |
| Classes | — |
| Pattern | — |
| Owner | Team |
| Logged | 2026-10-22 by JP |

**UML currently says:** `docs/images/` only has the activity and state diagram exports.
**Problem:** CONTRIBUTING.md requires both the `.vpp` and an exported image for every diagram, and the report needs the class diagram.
**Proposed change:** Export the class diagram (PNG or PDF) after the fixes above are applied, and again whenever it changes.

- [ ] UML updated (`.vpp` + export)
- [ ] Code updated
