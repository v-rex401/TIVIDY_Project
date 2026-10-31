/**
 * @file BuilderView.cpp
 * @brief Implementation of the build-your-own screen.
 * @author JP
 * @date 2026-10-31
 */

#include "BuilderView.h"

#include <algorithm>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <ftxui/component/event.hpp>

#include "FormParts.h"
#include "Scenarios.h"
#include "Theme.h"

using namespace ftxui;
using namespace FormParts;

namespace
{
    /** @brief Everything typed into the builder that isn't stored straight in the draft. */
    struct Fields
    {
        int step = 0;
        std::vector<std::string> steps = {"1 People", "2 Stages", "3 Routing", "4 Review and start"};

        std::string teamName, teamTitle, teamRank;
        int teamParent = 0;
        std::string personName, personTitle, personRank;
        int personTeam = 0;
        std::vector<std::string> parents; ///< "top level" then team names.
        std::vector<std::string> teams;   ///< "no team" then team names.

        std::string stageName, stageTasks;
        int builder = 0;
        std::vector<std::string> builders = {"Sequential", "Escalating"};

        int assignment = 0;
        std::vector<std::string> assignments = {"Role", "Workload", "Rule"};
        int routing = 2;
        std::vector<std::string> routings = {"Severity", "Approval chain", "Event"};
        std::string metricKey = "priority", startValue = "1", limit = "5";
        int normal = 0, emergency = 0, standard = 0, escalation = 0;
        std::string limit1 = "8", limit2 = "4", limit3 = "0";
        int stage1 = 0, stage2 = 0, stage3 = 0;
        std::vector<std::string> stageNames; ///< Names of the stages added so far.

        std::string message;
        bool messageOk = true;
    };

    /**
     * @brief Split "a, b ,c" into {"a", "b", "c"}.
     * @param text The comma-separated text.
     * @return The trimmed, non-empty parts.
     */
    std::vector<std::string> splitList(const std::string &text)
    {
        std::vector<std::string> parts;
        std::istringstream in(text);
        for (std::string part; std::getline(in, part, ',');)
        {
            std::size_t from = part.find_first_not_of(' ');
            std::size_t to = part.find_last_not_of(' ');
            if (from != std::string::npos)
            {
                parts.push_back(part.substr(from, to - from + 1));
            }
        }
        return parts;
    }

    /**
     * @brief A stage name from a picker index, or empty if there are no stages.
     * @param f The fields.
     * @param index The picker index.
     * @return The stage name.
     */
    std::string stageAt(const Fields &f, int index)
    {
        return index >= 0 && index < static_cast<int>(f.stageNames.size()) ? f.stageNames[index] : "";
    }

    /**
     * @brief Refresh the lists the pickers show from the draft.
     * @param draft The setup being built.
     * @param f The fields.
     */
    void refreshLists(const ScenarioSetup &draft, Fields &f)
    {
        f.parents = {"top level"};
        f.teams = {"no team"};
        for (const TeamSpec &t : draft.teams)
        {
            f.parents.push_back(t.name);
            f.teams.push_back(t.name);
        }
        f.stageNames.clear();
        for (const StageSpec &s : draft.stages)
        {
            f.stageNames.push_back(s.name);
        }
        int last = std::max(0, static_cast<int>(f.stageNames.size()) - 1);
        for (int *pick : {&f.normal, &f.emergency, &f.standard, &f.escalation, &f.stage1, &f.stage2, &f.stage3})
        {
            *pick = std::min(*pick, last);
        }
        f.teamParent = std::min(f.teamParent, static_cast<int>(f.parents.size()) - 1);
        f.personTeam = std::min(f.personTeam, static_cast<int>(f.teams.size()) - 1);
    }

    /**
     * @brief Point the routing pickers at sensible stages: urgent or high values to the last stage,
     *        normal work to the first. Called whenever the stage list changes.
     * @param f The fields.
     */
    void defaultPicks(Fields &f)
    {
        int last = std::max(0, static_cast<int>(f.stageNames.size()) - 1);
        f.normal = 0;
        f.emergency = last;
        f.standard = 0;
        f.escalation = last;
        f.stage1 = last;
        f.stage2 = last / 2;
        f.stage3 = 0;
    }

    /**
     * @brief Copy what the routing step chose into the draft, and fill in the parts that follow from the rest.
     * @param draft The setup being built.
     * @param f The fields.
     */
    void applyChoices(ScenarioSetup &draft, const Fields &f)
    {
        if (draft.name.empty())
        {
            draft.name = draft.workflowName.empty() ? "My workflow" : draft.workflowName;
        }
        draft.summary = "Built by hand in the TUI.";
        draft.builder = f.builders[f.builder];
        draft.assignment = kAssignmentKinds[f.assignment];

        // Escalation goes up by rank, lowest first, in the order people were added.
        std::vector<PersonSpec> byRank = draft.people;
        std::stable_sort(byRank.begin(), byRank.end(),
                         [](const PersonSpec &a, const PersonSpec &b) { return a.rank < b.rank; });
        draft.chain.clear();
        for (const PersonSpec &p : byRank)
        {
            draft.chain.push_back(p.name);
        }

        // One rule per job title, and one quick pick per title, so every option works straight away.
        std::set<std::string> titles;
        draft.rules.clear();
        draft.sampleWork.clear();
        for (const PersonSpec &p : draft.people)
        {
            if (titles.insert(p.title).second)
            {
                draft.rules.push_back({"requiredPosition", p.title, p.title});
                draft.sampleWork.push_back({"Routine task for a " + p.title, p.title, "Low"});
            }
        }
        if (!draft.people.empty())
        {
            draft.sampleWork.push_back({"Urgent task for a " + byRank.back().title, byRank.back().title, "Critical"});
        }

        RoutingSpec &r = draft.routing;
        r.kind = kRoutingKinds[f.routing];
        r.metricKey = f.metricKey;
        r.normalStage = stageAt(f, f.normal);
        r.emergencyStage = stageAt(f, f.emergency);
        r.approvalLimit = toInt(f.limit, 0);
        r.standardStage = stageAt(f, f.standard);
        r.escalationStage = stageAt(f, f.escalation);
        r.thresholds.clear();
        std::vector<std::pair<std::string, int>> rows = {{f.limit1, f.stage1}, {f.limit2, f.stage2}, {f.limit3, f.stage3}};
        for (const std::pair<std::string, int> &row : rows)
        {
            if (!row.first.empty() && !stageAt(f, row.second).empty())
            {
                r.thresholds.push_back({static_cast<double>(toInt(row.first, 0)), stageAt(f, row.second)});
            }
        }
        draft.startingData.clear();
        if (!f.metricKey.empty())
        {
            draft.startingData.push_back({f.metricKey, static_cast<double>(toInt(f.startValue, 0))});
        }
    }

    /**
     * @brief Set the routing step's fields from a complete setup (used by "load an example").
     * @param draft The setup.
     * @param f The fields.
     */
    void choicesFrom(const ScenarioSetup &draft, Fields &f)
    {
        refreshLists(draft, f);
        f.builder = indexOf(f.builders, draft.builder);
        f.assignment = indexOf(kAssignmentKinds, draft.assignment);
        f.routing = indexOf(kRoutingKinds, draft.routing.kind);
        f.metricKey = draft.routing.metricKey;
        f.startValue = draft.startingData.empty() ? "0" : number(draft.startingData.front().second);
        f.limit = number(draft.routing.approvalLimit);
        f.normal = indexOf(f.stageNames, draft.routing.normalStage);
        f.emergency = indexOf(f.stageNames, draft.routing.emergencyStage);
        f.standard = indexOf(f.stageNames, draft.routing.standardStage);
        f.escalation = indexOf(f.stageNames, draft.routing.escalationStage);
        std::vector<std::string *> limits = {&f.limit1, &f.limit2, &f.limit3};
        std::vector<int *> stages = {&f.stage1, &f.stage2, &f.stage3};
        for (std::size_t i = 0; i < 3; ++i)
        {
            bool has = i < draft.routing.thresholds.size();
            *limits[i] = has ? number(draft.routing.thresholds[i].limit) : "";
            *stages[i] = has ? indexOf(f.stageNames, draft.routing.thresholds[i].stage) : 0;
        }
    }

    /**
     * @brief A short labelled input.
     * @param label The label.
     * @param field The input.
     * @param width The input's width.
     * @return The element.
     */
    Element cell(const std::string &label, Component field, int width)
    {
        return vbox({text(label) | dim, field->Render() | size(WIDTH, EQUAL, width)});
    }
}

Component BuilderView(AppState &app)
{
    std::shared_ptr<Fields> f = std::make_shared<Fields>();
    ScenarioSetup &draft = app.draft;

    // Where the cursor goes back to after each add, so the next one can be typed straight away.
    std::shared_ptr<std::weak_ptr<ComponentBase>> firstTeamField = std::make_shared<std::weak_ptr<ComponentBase>>();
    std::shared_ptr<std::weak_ptr<ComponentBase>> firstPersonField = std::make_shared<std::weak_ptr<ComponentBase>>();
    std::shared_ptr<std::weak_ptr<ComponentBase>> firstStageField = std::make_shared<std::weak_ptr<ComponentBase>>();
    auto say = [f](const std::string &message, bool ok)
    {
        f->message = message;
        f->messageOk = ok;
    };

    // ----- step 1: people -----
    auto addTeam = [&draft, f, say, firstTeamField]
    {
        if (f->teamName.empty())
        {
            say("A team needs a name.", false);
            return;
        }
        draft.teams.push_back({f->teamName, f->teamTitle.empty() ? f->teamName : f->teamTitle, toInt(f->teamRank, 1),
                               f->teamParent == 0 ? "" : f->parents[f->teamParent]});
        say("Team " + f->teamName + " added.", true);
        f->teamName.clear();
        f->teamTitle.clear();
        f->teamRank.clear();
        refreshLists(draft, *f);
        if (Component field = firstTeamField->lock())
        {
            field->TakeFocus();
        }
    };
    auto addPerson = [&draft, f, say, firstPersonField]
    {
        if (f->personName.empty() || f->personTitle.empty())
        {
            say("A person needs a name and a job title.", false);
            return;
        }
        draft.people.push_back({f->personName, f->personTitle, toInt(f->personRank, 1),
                                f->personTeam == 0 ? "" : f->teams[f->personTeam]});
        say(f->personName + " added. The team is kept for the next person.", true);
        f->personName.clear();
        f->personTitle.clear();
        f->personRank.clear();
        if (Component field = firstPersonField->lock())
        {
            field->TakeFocus();
        }
    };

    InputOption teamInput = Theme::input();
    teamInput.on_enter = addTeam;
    InputOption personInput = Theme::input();
    personInput.on_enter = addPerson;

    Component teamName = Input(&f->teamName, "e.g. Ward", teamInput);
    Component teamTitle = Input(&f->teamTitle, "e.g. Nursing", teamInput);
    Component teamRank = Input(&f->teamRank, "1", teamInput);
    *firstTeamField = teamName;
    Component teamParent = Theme::choice(&f->parents, &f->teamParent);
    Component personName = Input(&f->personName, "e.g. Jane Mokoena", personInput);
    Component personTitle = Input(&f->personTitle, "e.g. Nurse", personInput);
    Component personRank = Input(&f->personRank, "1", personInput);
    *firstPersonField = personName;
    Component personTeam = Theme::choice(&f->teams, &f->personTeam);
    Component addTeamButton = Button("Add team", addTeam, Theme::button());
    Component addPersonButton = Button("Add person", addPerson, Theme::button());
    Component undoPerson = Button("Remove last person", [&draft, say]
    {
        if (!draft.people.empty())
        {
            say(draft.people.back().name + " removed.", true);
            draft.people.pop_back();
        }
    }, Theme::button());
    Component undoTeam = Button("Remove last team", [&draft, f, say]
    {
        if (!draft.teams.empty())
        {
            say("Team " + draft.teams.back().name + " removed.", true);
            draft.teams.pop_back();
            refreshLists(draft, *f);
        }
    }, Theme::button());

    Component peopleStep = Container::Vertical({
        Container::Horizontal({personName, personTitle, personRank}), personTeam,
        Container::Horizontal({addPersonButton, undoPerson}),
        Container::Horizontal({teamName, teamTitle, teamRank}), teamParent,
        Container::Horizontal({addTeamButton, undoTeam}),
    });

    // ----- step 2: stages -----
    auto addStage = [&draft, f, say, firstStageField]
    {
        if (f->stageName.empty())
        {
            say("A stage needs a name.", false);
            return;
        }
        draft.stages.push_back({f->stageName, splitList(f->stageTasks)});
        say("Stage " + f->stageName + " added with " + std::to_string(draft.stages.back().tasks.size()) + " tasks.", true);
        f->stageName.clear();
        f->stageTasks.clear();
        refreshLists(draft, *f);
        defaultPicks(*f);
        if (Component field = firstStageField->lock())
        {
            field->TakeFocus();
        }
    };
    InputOption stageInput = Theme::input();
    stageInput.on_enter = addStage;
    Component workflowName = Input(&draft.workflowName, "e.g. Onboarding", Theme::input());
    Component builder = Theme::choice(&f->builders, &f->builder);
    Component stageName = Input(&f->stageName, "e.g. Review", stageInput);
    Component stageTasks = Input(&f->stageTasks, "tasks, separated by commas", stageInput);
    *firstStageField = stageName;
    Component addStageButton = Button("Add stage", addStage, Theme::button());
    Component undoStage = Button("Remove last stage", [&draft, f, say]
    {
        if (!draft.stages.empty())
        {
            say("Stage " + draft.stages.back().name + " removed.", true);
            draft.stages.pop_back();
            refreshLists(draft, *f);
            defaultPicks(*f);
        }
    }, Theme::button());
    Component stagesStep = Container::Vertical({
        workflowName, builder, stageName, stageTasks, Container::Horizontal({addStageButton, undoStage}),
    });

    // ----- step 3: routing -----
    Component assignment = Theme::choice(&f->assignments, &f->assignment);
    Component routing = Theme::choice(&f->routings, &f->routing);
    Component metricKey = Input(&f->metricKey, "e.g. priority", Theme::input());
    Component startValue = Input(&f->startValue, "0", Theme::input());
    Component normal = Theme::choice(&f->stageNames, &f->normal);
    Component emergency = Theme::choice(&f->stageNames, &f->emergency);
    Component limit = Input(&f->limit, "5", Theme::input());
    Component standard = Theme::choice(&f->stageNames, &f->standard);
    Component escalation = Theme::choice(&f->stageNames, &f->escalation);
    Component limit1 = Input(&f->limit1, "", Theme::input());
    Component limit2 = Input(&f->limit2, "", Theme::input());
    Component limit3 = Input(&f->limit3, "", Theme::input());
    Component stage1 = Theme::choice(&f->stageNames, &f->stage1);
    Component stage2 = Theme::choice(&f->stageNames, &f->stage2);
    Component stage3 = Theme::choice(&f->stageNames, &f->stage3);
    Component routingStep = Container::Vertical({
        assignment, Container::Horizontal({metricKey, startValue}), routing, normal, emergency, limit, standard,
        escalation, Container::Horizontal({limit1, stage1}), Container::Horizontal({limit2, stage2}),
        Container::Horizontal({limit3, stage3}),
    });

    // ----- step 4: review -----
    auto start = [&app, &draft, f, say]
    {
        applyChoices(draft, *f);
        std::vector<std::string> problems = draft.problems();
        if (!problems.empty())
        {
            say("Not ready yet: " + problems.front(), false);
            return;
        }
        app.start(draft);
    };
    Component startButton = Button("Start running it", start, Theme::button());
    Component reviewStep = Container::Vertical({startButton});

    Component steps = Container::Tab({peopleStep, stagesStep, routingStep, reviewStep}, &f->step);
    Component stepPicker = Theme::choice(&f->steps, &f->step);
    Component example = Button("Load the hospital example to edit", [&draft, f, say]
    {
        draft = Scenarios::hospital();
        draft.name = "My copy of the hospital";
        choicesFrom(draft, *f);
        say("Loaded. Change anything, then go to 4 to start it.", true);
    }, Theme::button());
    Component back = Button("Back to the start screen", [&app] { app.screen = PickerScreen; }, Theme::button());
    Component next = Button("Next step ›", [f] { f->step = std::min(f->step + 1, 3); }, Theme::button());
    // Start with the cursor in the first field, not on the step names above it.
    std::shared_ptr<int> focused = std::make_shared<int>(1);
    Component layout = Container::Vertical({stepPicker, steps, Container::Horizontal({next, example, back})}, focused.get());

    Component view = Renderer(layout, [&draft, f, focused, stepPicker, steps, next, example, back,
                                       personName, personTitle, personRank, personTeam, addPersonButton, undoPerson,
                                       teamName, teamTitle, teamRank, teamParent, addTeamButton, undoTeam,
                                       workflowName, builder, stageName, stageTasks, addStageButton, undoStage,
                                       assignment, routing, metricKey, startValue, normal, emergency, limit, standard,
                                       escalation, limit1, limit2, limit3, stage1, stage2, stage3, startButton]
    {
        refreshLists(draft, *f);
        Element form;
        Elements preview;

        if (f->step == 0)
        {
            form = vbox({
                text("A person") | bold,
                hbox({cell("Name", personName, 22), text(" "), cell("Job title", personTitle, 18), text(" "),
                      cell("Rank", personRank, 5)}),
                row("Team", personTeam->Render()),
                hbox({addPersonButton->Render(), text("  "), undoPerson->Render()}),
                text(""),
                text("A team (optional)") | bold,
                hbox({cell("Name", teamName, 22), text(" "), cell("Title", teamTitle, 18), text(" "),
                      cell("Rank", teamRank, 5)}),
                row("Inside", teamParent->Render()),
                hbox({addTeamButton->Render(), text("  "), undoTeam->Render()}),
                text(""),
                paragraph("Enter in a field adds it. Rank is authority: a request needing rank 3 climbs the chain "
                          "until someone with rank 3 or more takes it.") | dim,
            });
            for (const TeamSpec &t : draft.teams)
            {
                preview.push_back(text("▸ " + t.name + "  rank " + std::to_string(t.rank) +
                                       (t.parent.empty() ? "" : "  in " + t.parent)) | bold);
                for (const PersonSpec &p : draft.people)
                {
                    if (p.team == t.name)
                    {
                        preview.push_back(text("    " + p.name + "  " + p.title + ", rank " + std::to_string(p.rank)));
                    }
                }
            }
            for (const PersonSpec &p : draft.people)
            {
                if (p.team.empty())
                {
                    preview.push_back(text("· " + p.name + "  " + p.title + ", rank " + std::to_string(p.rank)));
                }
            }
            if (preview.empty())
            {
                preview.push_back(text("Nobody yet. Add a person on the left.") | dim);
            }
        }
        else if (f->step == 1)
        {
            form = vbox({
                row("Workflow name", workflowName->Render()),
                row("Builder", builder->Render()),
                hbox({text("") | size(WIDTH, EQUAL, 15),
                      paragraph(f->builder == 0 ? "Links each stage to the next, in order."
                                                : "Links each stage back to the one before it.") | dim}),
                text(""),
                text("A stage") | bold,
                row("Name", stageName->Render()),
                row("Tasks", stageTasks->Render()),
                hbox({addStageButton->Render(), text("  "), undoStage->Render()}),
                text(""),
                paragraph("The Director tells the builder to make these stages in this order. Every case gets its "
                          "own copy.") | dim,
            });
            for (const StageSpec &s : draft.stages)
            {
                preview.push_back(text(s.name) | bold);
                for (const std::string &task : s.tasks)
                {
                    preview.push_back(text("   " + task));
                }
            }
            if (preview.empty())
            {
                preview.push_back(text("No stages yet.") | dim);
            }
        }
        else if (f->step == 2)
        {
            bool none = f->stageNames.empty();
            form = vbox({
                row("Who gets work", assignment->Render()),
                hbox({cell("Value routing reads", metricKey, 22), text(" "), cell("Each case starts at", startValue, 10)}),
                row("Default routing", routing->Render()),
                none ? text("Add stages in step 2 first; routing sends work to them.") | color(Theme::refused())
                     : text(""),
                text("Severity") | bold,
                row("normal work to", normal->Render()),
                row("urgent work to", emergency->Render()),
                text("Approval chain") | bold,
                row("limit", limit->Render() | size(WIDTH, EQUAL, 10)),
                row("up to it, to", standard->Render()),
                row("above it, to", escalation->Render()),
                text("Event (first match wins)") | bold,
                hbox({text("at least ") | dim, limit1->Render() | size(WIDTH, EQUAL, 6), text(" → "), stage1->Render()}),
                hbox({text("at least ") | dim, limit2->Render() | size(WIDTH, EQUAL, 6), text(" → "), stage2->Render()}),
                hbox({text("at least ") | dim, limit3->Render() | size(WIDTH, EQUAL, 6), text(" → "), stage3->Render()}),
            });
            preview.push_back(paragraph("All three routing settings are kept, so any of them can be picked when work "
                                        "is added. The default is what the Add work form starts on.") | dim);
            preview.push_back(text(""));
            preview.push_back(paragraph("Role: the first person whose title matches. Workload: whoever has least. "
                                        "Rule: one rule per job title is made for you.") | dim);
        }
        else
        {
            ScenarioSetup check = draft;
            applyChoices(check, *f);
            std::vector<std::string> problems = check.problems();
            Elements list;
            for (const std::string &p : problems)
            {
                list.push_back(text("✗ " + p) | color(Theme::refused()));
            }
            if (problems.empty())
            {
                list.push_back(text("Everything lines up.") | color(Theme::accent()) | bold);
            }
            form = vbox({
                vbox(list),
                text(""),
                startButton->Render(),
                text(""),
                paragraph("Starting builds everything with the engine: the staff and chain, the definition via the "
                          "Director and builder, then opens the first case.") | dim,
            });
            preview = {
                text(std::to_string(check.people.size()) + " people, " + std::to_string(check.teams.size()) +
                     " teams, " + std::to_string(check.stages.size()) + " stages"),
                text("Builder: " + check.builder),
                text("Assignment: " + check.assignment),
                text("Routing: " + check.routing.kind + " on \"" + check.routing.metricKey + "\""),
            };
            std::string chain;
            for (const std::string &name : check.chain)
            {
                chain += (chain.empty() ? "" : " → ") + name;
            }
            preview.push_back(paragraph("Escalation chain, by rank: " + chain) | dim);
        }

        Element message = f->message.empty()
                              ? text("")
                              : text(f->message) | color(f->messageOk ? Theme::accent() : Theme::refused());
        return vbox({
            hbox({text(" TIVIDY ") | bold | color(Theme::accent()), text(" Build my own workflow   ") | bold,
                  stepPicker->Render(), filler()}),
            separatorLight(),
            hbox({
                Theme::panel("Add", form) | flex,
                Theme::panel("So far", vbox(preview) | yframe) | size(WIDTH, EQUAL, 50),
            }) | flex,
            hbox({next->Render(), text("  "), example->Render(), text("  "), back->Render()}),
            separatorLight(),
            hbox({text(" "), message}),
            hbox({text(" "), Theme::keyHint("tab / ↑↓", "move"), Theme::keyHint("←→", "choose"),
                  Theme::keyHint("enter", "add"), Theme::keyHint("pgup/pgdn", "step"), Theme::keyHint("esc", "back"),
                  filler()}),
        });
    });

    return CatchEvent(view, [&app, f](Event event)
    {
        if (event == Event::Escape)
        {
            app.screen = PickerScreen;
            return true;
        }
        if (event == Event::PageDown)
        {
            f->step = std::min(f->step + 1, 3);
            return true;
        }
        if (event == Event::PageUp)
        {
            f->step = std::max(f->step - 1, 0);
            return true;
        }
        return false;
    });
}
