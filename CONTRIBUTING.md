# Contributing to TIVIDY

**Team:** Git Push and Pray

This document defines how we work on TIVIDY as a team.

These are the rules that will get a pull request rejected.

1. **Never commit directly to `main` or `dev`.** Work on a branch, open a PR.
2. **The generic engine may not know about the scenarios.**
3. **Approvals and green CI before merge.** `dev` needs **2 approvals**; `main` needs **4**. At least one approval must come from an owner of a subsystem you touched.
4. **New behaviour needs a test.** Existing tests must still pass.
5. **If the code changes the design, update the UML in the same PR.**
6. **Review within 12 hours of being requested.**
7. **Doxygen comments on new public members, written with the code.**
8. **Tell people before you change a shared interface.**
9. **One logical change per commit, with a real commit message.**
10. **Blocked? Say so the same day.**


## 1. Our working principles

The following principles apply throughout the project:

### 1.1 The shared repository belongs to the team

Do not treat your branch or subsystem as completely separate from the rest of the project. Your work may affect another person's code, diagrams, tests, or pattern implementation.

**MUST:**
- Communicate changes that affect another person's subsystem.
- Keep shared interfaces stable unless the change has been discussed.
- Avoid unnecessary changes to files you are not responsible for.
- Leave the repository in a usable state.

### 1.2 Working code is not automatically good code

A solution may work and still need to be changed if it:
- violates the intended design pattern
- puts responsibility in the wrong class
- introduces unnecessary coupling
- duplicates behaviour that should be polymorphic
- makes the generic workflow engine depend on the scenarios
- contradicts the agreed UML
- makes another workflow scenario unnecessarily difficult to implement.

When reviewing work, ask both:

> **Does it work?**

and

> **Is this the design we agreed to build?**

### 1.3 Ask rather than assume

If a requirement, interface, UML relationship, or design decision is unclear, ask the relevant team member before implementing it.

Do not spend hours implementing an assumption that could have been resolved in a short discussion.

### 1.4 Respect the person, review the work

Disagreements about implementation are normal. Discuss the design, evidence, requirements, and trade-offs rather than the person who wrote the code.

---

## 2. Repository layout

```text
/
├── README.md
├── CONTRIBUTING.md
├── CMakeLists.txt           <- real build definition
├── Makefile                 <- thin wrapper over CMake
├── Dockerfile
├── docker-compose.yml
├── Doxyfile
├── .gitignore
├── .dockerignore
├── include/                 <- all .h files
├── src/                     <- all .cpp files, incl. main.cpp
├── tests/                   <- GoogleTest files
├── build/                   <- generated — DO NOT COMMIT
├── report/                  <- final PDF report
├── docs/
│   ├── images/              <- diagram exports (.png/.jpg/.pdf)
│   ├── vpp/                 <- Visual Paradigm source files (.vpp)
│   ├── research/            <- Task 1 research + references
│   ├── scenario/            <- Task 2 scope and scenario
│   ├── patterns/            <- pattern justifications and participant tables
│   ├── decisions/           <- design decision log
│   └── html/                <- generated Doxygen — DO NOT COMMIT
└── .github/
    └── workflows/
        ├── build.yml
        ├── valgrind.yml
        └── tests.yml
```

### Documentation rule

**MUST:** Every Markdown document belonging to the project lives in `docs/`, except root-level project files such as `README.md` and `CONTRIBUTING.md`.

Do not commit:
- Word documents
- screenshots of text instead of the actual document
- unrelated personal notes
- generated output (`build/`, `docs/html/`, `html/`, `latex/`)

For diagrams, commit both:
1. the editable source file and
2. the exported image/PDF

---

## 3. Before starting a task

Before creating a branch, make sure you understand what you are changing

### Checklist

- [ ] Check the task/issue or agreed requirement.
- [ ] Check the current UML/design.
- [ ] Check whether someone else is already changing the relevant files.
- [ ] Identify which pattern(s) the task involves.
- [ ] Identify interfaces or classes your work depends on.
- [ ] Tell affected team members if your work may change their subsystem.
- [ ] Start from the latest `dev`. (git pull)

---

## 4. Branching strategy

GitFlow-style workflow.

`Vashti` will performs the `dev` → `main` merges and configures branch protection.

### Permanent branches

| Branch | Purpose | Approvals to merge |
|---|---|---|
| `main` | Submission-ready code and tagged releases | **4** |
| `dev` | Integration branch for completed work | **2** |

### Protected branches

**MUST:** Never commit directly to `main` or `dev`,

All changes must come through a pull request from a short-lived branch.

Required status checks on both branches: **Build**, **Valgrind**, **Tests**.

### Branch naming

```text
feature/<subsystem>-<short-description>
fix/<subsystem>-<short-description>
docs/<document>
test/<subsystem>
```

Examples:

```text
feature/routing-workload-strategy
fix/lifecycle-memento-restore
docs/activity-diagram-1
test/escalation-chain
```

Use lowercase and hyphens. Do not use personal names in branch names.

### Creating a branch

Start from the latest `dev`:

```bash
git checkout dev
git pull origin dev
git checkout -b feature/my-branch
```

### Updating your branch

Before opening a PR:

```bash
git checkout dev
git pull origin dev
git checkout feature/my-branch
git merge dev
```

Resolve conflicts on your branch, not directly in the shared branch.

### Merge strategy

**Use a merge commit when merging into `dev`. Do not squash by default.**

A squash merge collapses every commit on your branch into a single commit on `dev`, which destroys the individual commit history we are being assessed on.
With merge commits, each person's commits remain visible and attributed in the history of `dev` and `main`.
Rebase-merging is not used, because it rewrites commit hashes and makes the shared history harder to reason about.

---

## 5. Commits

### What makes a good commit?

A commit should:

- represent one logical change
- have a clear message
- avoid unrelated changes
- leave the project buildable where reasonably possible
- not contain debugging code
- not contain generated files or personal configuration

> **One commit should answer one clear question: "What changed?"**

Each member needs **at least 10 meaningful commits** across the project.

### When to commit

Good points to commit include:

- a method has been implemented
- a test has been added
- a bug has been fixed
- a document section is complete
- a diagram has been updated.
Do not batch an entire day's unrelated work into one commit.

### Commit message format

Types:

```text
feat  fix  test  docs  refactor  build  chore
```

Good:

```text
feat(routing): add workload-based assignment strategy
test(lifecycle): cover Memento restore after failed command
fix(escalation): forward to successor when handler returns false
docs(patterns): justify Builder for definition construction
build(ci): add GoogleTest workflow
```

Bad:

```text
update
fixed stuff
Anke's changes
work on project
stuff
final
```

---

## 6. Build, run and test

### With Docker

```bash
docker compose run tividy  # run the program
docker compose run tests # run the test suite
```

The first run takes ~2 minutes while cmake is installed in the image.

### Locally

Requires `g++` (C++17), `cmake` (3.14+) and `make`. CMake fetches FTXUI and GoogleTest on the first build.

```bash
make  # build
make run # build and run
make test # build and run the test suite
make docs # generate Doxygen into docs/html/
make clean # remove build output
```

### Testing requirements

- Framework: **GoogleTest** (fetched automatically by CMake — do not install it manually).
- Test files live in `tests/`, named `<Thing>Test.cpp`.
- **Every team member must contribute tests**.
- Every PR that adds behaviour must add or update relevant tests.
- Every pattern participant should have tests demonstrating its intended behaviour.
- Existing tests must continue to pass.

Test important workflow behaviour specifically: transitions, routing, **instance
isolation** (changing one instance must not affect another) and invalid
operations.

For example, a Strategy test should demonstrate that changing the strategy changes the result without changing the caller.

### Test behaviour, not just compilation

Compilation proves the code is syntactically and structurally valid. It does **not** prove the implementation behaves correctly. A useful test demonstrates the behaviour the requirement actually cares about.

### CI

Three workflows run on every PR to `main` and `dev`:

| Workflow | What it does |
|---|---|
| **Build** | Builds `tividy` with CMake (C++17, `-Wall -Wextra` on `src/`), runs it with stdin closed |
| **Valgrind** | Builds `tividy` with CMake in Debug, runs it under `--leak-check=full --error-exitcode=1` |
| **Tests** | Runs `make test` — CMake, GoogleTest, ctest |

A red CI build blocks the merge.

If CI fails for infrastructure reasons or a known unrelated issue, the team must agree on how to proceed and record the reason. A known failure must still be resolved.

---

## 7. Documentation (Doxygen)

Doxygen comments are **compulsory**, written with the code, not afterwards.

Follow the course Doxygen Comment Guide:
- `/** ... */`, never `/* ... */`
- `@tag`, not `\tag`
- File header (`@file`, `@brief`, `@author`, `@date`) on every new file
- Every public function: `@brief`, `@param` per parameter, `@return` if it returns, `@throws` if it can throw
- `///<` for short inline comments after member variables

**For pattern participants, name the role in the `@brief`:**

```cpp
/**
 * @brief Director participant of the Builder pattern.
 *
 * Owns the construction algorithm and issues buildXxx() calls to a
 * WorkflowBuilder in the correct order (Ch.22 §22.5.3 — the algorithm
 * must live in a separate Director class).
 */
class WorkflowDirector { ... };
```

Before opening a PR, run `make docs` and check the warnings — `WARN_IF_UNDOCUMENTED` lists every undocumented member.

---

## 8. Definition of Done

- [ ] The implementation satisfies the agreed requirement.
- [ ] The code compiles without warnings.
- [ ] Relevant tests have been added or updated.
- [ ] Existing tests still pass.
- [ ] Doxygen comments are present for new public members.
- [ ] Relevant UML diagrams have been updated (both `.vpp` and export).
- [ ] Pattern responsibilities are correctly represented.
- [ ] The generic engine remains scenario-independent.
- [ ] No unnecessary coupling has been introduced.
- [ ] The branch has been pushed.
- [ ] The PR description explains what changed and why.
- [ ] The required reviewers have approved.
- [ ] Requested review changes have been addressed.

> **"It works on my machine" is not the definition of done.**

---

## 9. Pull requests

### Requirements to merge

1. **2 approving reviews for `dev`, 4 for `main`**, with at least one from an owner of a subsystem the PR touches.
2. All three CI checks green.
3. The project builds without warnings.
4. Relevant tests pass.
5. No unresolved merge conflicts.
6. The PR description is complete.
7. Relevant documentation/UML updated.
8. The branch is deleted after merging.

The project needs **at least 3 closed pull requests** overall.

### Who reviews, and by when

**Request two reviewers when you open the PR** (four for a `main` PR):

1. the owner of each subsystem your PR touches, and
2. another person you assign it to.

On GitHub: open the PR → **Reviewers** in the right-hand sidebar → pick the people → they get notified.

**Reviews are due within 12 hours of being requested.** If you cannot review in time, say so in the PR immediately so the author can request someone else.

Silence is the failure mode — an unreviewed PR blocks the branch it came from and everything built on top of it.

If a PR has been open for more than 24 hours without the required approvals, raise it in the group chat.

### PR description template

```markdown
## What

One or two sentences describing the change.

## Why

Which requirement, feedback point, bug, or design decision does this address?

## Patterns touched

e.g. Strategy (AssignmentStrategy), Composite (Staff hierarchy)

## Design impact

Describe any changes to responsibilities, interfaces, or relationships.

## Diagram impact

- [ ] No diagram changes needed
- [ ] Class diagram updated
- [ ] Sequence/activity/state diagram updated

## Testing

Commands used and relevant test results.

## How to verify

Explain how another team member can verify the behaviour.
```

### Before opening a PR

```bash
make clean
make
make test
make docs
```

Then check:

- [ ] My branch is based on the latest `dev`.
- [ ] The project builds successfully.
- [ ] All tests pass.
- [ ] New behaviour has tests.
- [ ] No generated files committed (`build/`, `docs/html/`, `html/`, `latex/`).
- [ ] UML matches the implementation.
- [ ] Doxygen is updated.
- [ ] The genericness rule is still satisfied.
- [ ] The PR contains only relevant changes.
- [ ] I have requested the required reviewers.
- [ ] I can explain why the change was designed this way.

---

## 10. Code review

**Turnaround: 12 hours.**

A reviewer should check:

### Correctness

- Does the implementation satisfy the requirement?
- Does the code actually behave correctly?
- Are edge cases handled where relevant?

### Design

- Does the implementation match the UML?
- Is the correct design pattern being used?
- Are responsibilities assigned to the right classes?
- Is there unnecessary coupling?
- Are there `if`/`switch` chains doing work that should be handled through polymorphism?

### Genericness

No class in `src/` or `include/` may reference the scenarios or any other industry. Scenario detail arrives as strings and data through the Director and the TUI.

### Documentation

- Are public members documented?
- Does the `@brief` name the pattern role where relevant?
- Do diagrams still match the code?

### Testing

- Is there a test?
- Does the test prove behaviour rather than merely compilation?

## A meaningful review

Do not approve a PR simply because you trust the person who wrote it.

If something is unclear, potentially incorrect, or inconsistent with the design, leave a comment.

---

## 11. Subsystem ownership (TBD)

| Subsystem | Key classes | Patterns | Owner |
|---|---|---|---|

# 11. Git mistakes

If you commit to the wrong branch:
1. Stop.
2. Tell the relevant team member.
3. Determine whether the commit can be moved safely.
4. Fix it deliberately.

# Acknowledgement

This document is maintained through pull requests.

Approving a PR that introduces or materially changes this document means that the team member has read the current standards and agrees to follow them.
