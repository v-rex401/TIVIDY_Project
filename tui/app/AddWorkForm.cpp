/**
 * @file AddWorkForm.cpp
 * @brief The "Add work" pop-up: quick picks or a typed task, with a choice of strategies.
 * @author JP
 * @date 2026-10-31
 */

#include "Forms.h"

#include <string>
#include <vector>

#include <ftxui/component/event.hpp>

#include "WorkflowInstance.h"
#include "FormParts.h"
#include "Theme.h"

using namespace ftxui;
using namespace FormParts;

Popup AddWorkForm(AppState &app, std::shared_ptr<MainUi> ui)
{
    /** @brief The form's fields. */
    struct Fields
    {
        std::string name;
        std::string role;
        int severity = 0;
        int assignment = 0;
        int routing = 0;
        int pick = 0;
        std::vector<std::string> picks;
        std::vector<std::string> severities = {"Low", "Medium", "High", "Critical"};
        std::vector<std::string> assignments = {"Role", "Workload", "Rule"};
        std::vector<std::string> routings = {"Severity", "Approval chain", "Event"};
    };
    std::shared_ptr<Fields> f = std::make_shared<Fields>();

    auto submit = [&app, ui, f](const WorkSpec &work)
    {
        SubmitResult result = app.session->addWork(app.currentCase(), work, kAssignmentKinds[f->assignment],
                                                    kRoutingKinds[f->routing]);
        app.say(result.message, result.accepted);
        if (result.accepted)
        {
            ui->lastWork = work.name;
            ui->addWork = false;
        }
    };
    auto submitTyped = [f, submit] { submit({f->name, f->role, f->severities[f->severity]}); };
    auto submitPick = [&app, f, submit]
    {
        const std::vector<WorkSpec> &samples = app.session->setup().sampleWork;
        if (f->pick >= 0 && f->pick < static_cast<int>(samples.size()))
        {
            submit(samples[f->pick]);
        }
    };

    MenuOption pickOption = MenuOption::Vertical();
    pickOption.on_enter = submitPick;
    pickOption.entries_option.transform = [](const EntryState &state)
    {
        std::size_t bar = state.label.find('|');
        Element row = hbox({text(state.active ? " › " : "   ") | color(Theme::accent()),
                            text(state.label.substr(0, bar)) | size(WIDTH, EQUAL, 28),
                            text(bar == std::string::npos ? "" : state.label.substr(bar + 1)) | dim});
        return state.focused ? row | inverted : row;
    };
    Component picks = Menu(&f->picks, &f->pick, pickOption);

    InputOption typed = Theme::input();
    typed.on_enter = submitTyped;
    Component name = Input(&f->name, "e.g. Check vitals", typed);
    Component role = Input(&f->role, "a job title, e.g. Nurse", typed);
    Component severity = Theme::choice(&f->severities, &f->severity);
    Component assignment = Theme::choice(&f->assignments, &f->assignment);
    Component routing = Theme::choice(&f->routings, &f->routing);
    Component add = Button("Add", submitTyped, Theme::button());
    Component cancel = Button("Cancel", [ui] { ui->addWork = false; }, Theme::button());

    Component form = Container::Vertical({picks, name, role, severity, assignment, routing,
                                          Container::Horizontal({add, cancel})});

    Component view = Renderer(form, [&app, f, picks, name, role, severity, assignment, routing, add, cancel]
    {
        WorkflowInstance *c = app.currentCase();
        const ScenarioSetup &setup = app.session->setup();
        Element content = vbox({
            text("Quick picks: enter adds it straight away") | dim,
            picks->Render(),
            separatorLight(),
            text("Or describe your own") | dim,
            row("What", name->Render()),
            row("Needs", role->Render()),
            row("Severity", severity->Render()),
            separatorLight(),
            row("Who gets it", assignment->Render()),
            hbox({text(std::string(15, ' ')), paragraph(assignmentHint(setup, kAssignmentKinds[f->assignment])) | dim}),
            row("Where it goes", routing->Render()),
            hbox({text(std::string(15, ' ')), paragraph(routingHint(setup, kRoutingKinds[f->routing], c)) | dim}),
            separatorLight(),
            hbox({add->Render(), text("  "), cancel->Render(), filler(), text("esc closes") | dim}),
        });
        return frame("Add work to " + (c != nullptr ? c->getId() : std::string("?")), content, 78);
    });

    auto onOpen = [&app, f]
    {
        const ScenarioSetup &setup = app.session->setup();
        f->picks.clear();
        for (const WorkSpec &work : setup.sampleWork)
        {
            f->picks.push_back(work.name + "|" + work.role + ", " + work.severity);
        }
        f->name.clear();
        f->role.clear();
        f->severity = 0;
        f->pick = 0;
        f->assignment = indexOf(kAssignmentKinds, setup.assignment);
        f->routing = indexOf(kRoutingKinds, setup.routing.kind);
    };
    return {view, onOpen};
}
