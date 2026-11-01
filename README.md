# TIVIDY Project - COS214

## Team Members
Team **Git Push and Pray**

| Name | Student Number |Main responsibility |
|---|---|---|
| Azolile Mbanga | u24820360 | WorkflowManager, Strategy subsystem |
| Anke de Frey | u24611400 | Adapter and notifications, event log and history, CSV persistence |
| Vashti Pillay | u25041887 | Visitor subsystem, testing and integration |
| Kayla Falconer | u25006747 | Builder, Chain of Responsibility, Composite |
| Jean-Pierre Joubert | u25056809 | WorkflowFacade, FTXUI terminal interface |
| Ronin Sydney | u25056507 | Command and Observer subsystems |
| Jezelle Govender | u25176383 | State, Composite, WorkflowInstance |

## Design Overview
TIVIDY is a generic workflow engine.
It knows about workflow definitions, cases, stages, work items, people and state changes, but nothing about any particular industry.

The code is in three layers:

- **`src/` - the engine.**
    No industry terms, no terminal code.
    Scenario detail reaches it as strings and numbers through the Director and the strategies.
- **`tui/core/` — the session.**
    Turns a scenario description into engine objects and exposes one method per user action.
- **`tui/app/` — the screens.**
    FTXUI only. Draws what the session returns.

## Build, Run and Test

### With Docker

```bash
docker compose build # build the image
docker compose run --rm tividy # run the program (opens the TUI)
docker compose run --rm tests # run the test suite
```

The first build takes a few minutes while CMake, FTXUI and GoogleTest are set up in the image. `run` doesn't rebuild the image, so run `docker compose build` again after changing code.

### Locally

Requires `g++` (C++17), `cmake` (3.14+) and `make`. CMake fetches FTXUI (the TUI library) and GoogleTest on the first build, don't install them yourself.

```bash
make # build
make run # build and run
make test # build and run tests
make clean # remove build output
```

### Using the TUI

`make run` opens the full-screen TUI. See [tui/README.md](tui/README.md) for the keys, a demo script, how it is built and the engine changes it needed.

### The two industries

Both run on the same engine with no recompile — press `s` in the TUI to switch.

| Scenario | Industry | Stages | Routing decision | Exception path |
|---|---|---|---|---|
| Hospital admissions | Healthcare | Admission -> Assessment -> Emergency Care -> Treatment -> Discharge | Event-based on the triage score: raise it above the threshold and the next work item is routed to Emergency Care instead of Assessment | Reject sends the case back to Pending, approving a Completed case is refused, Cancel ends it |
| Insurance claims | Insurance | Lodgement -> Assessment -> Senior Review -> Settlement | Approval-chain on the claim amount: above the limit, work goes to Senior Review | As above, plus escalation up the staff chain when nobody of sufficient rank can approve |

To see each one end to end:

```bash
make run        # opens on the first scenario
                # s         switch scenario
                # n         new case
                # w         add work      (assignment + routing + notification)
                # v         change a value, then w again to see routing change
                # a / r / c approve, reject, cancel
                # u         undo the last command
                # e         escalate
                # p         reports
                # 4         history log
```

```bash
./build/tividy < /dev/null
```

### Generic engine classes reused by both industries

Neither scenario changes any of these; the differences arrive as data through the `WorkflowDirector` and the strategies:

`WorkflowFacade`, `WorkflowManager`, `WorkflowDefinition`, `WorkflowInstance`,`Stage`, `Task`, `WorkItem`, `Staff`, `Employee`, `Team`, `State` and its four concrete states, `Command` and its four commands, `Invoker`, `CommandHistory`, `Memento`, `EventLog`, `WorkflowEvent`, the `AssignmentStrategy` and `RoutingStrategy` families, both visitors, both observers, and the `ExternalSystem` / `NotificationAdapter` pair.

## Dependencies

| Dependency | Version | Why |
|---|---|---|---|
| [FTXUI](https://github.com/ArthurSonzogni/FTXUI) | 6.1.9 | Terminal user interface |
| [GoogleTest](https://github.com/google/googletest) | 1.17.0 | Unit tests |

Both are fetched by CMake on the first build.

The project is built as **C++17** rather than C++11, with permission, because FTXUI requires it.
But ALL design pattern implementation (src/) is C++11 compliant and uses no C++14 or C++17 language features.

## Engineering

- **CI** — three GitHub Actions workflows run on every pull request to `main` and `dev`: Build (compiles with `-Wall -Wextra` and runs the program),
    Tests (`make test`), and Valgrind (`--leak-check=full --error-exitcode=1`).
- **Docker** — `docker compose run --rm tividy` runs the TUI in a container with no local toolchain, and `docker compose run --rm tests` runs the suite.

### Documentation

```bash
doxygen Doxyfile # output in docs/html/index.html
```