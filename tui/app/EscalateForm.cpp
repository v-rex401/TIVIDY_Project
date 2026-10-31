/**
 * @file EscalateForm.cpp
 * @brief The "Escalate" pop-up: send a request up the staff chain.
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

Popup EscalateForm(AppState &app, std::shared_ptr<MainUi> ui)
{
    /** @brief The form's fields. */
    struct Fields
    {
        int start = 0;
        std::string rank;
        std::vector<std::string> starts;
        std::vector<std::string> workNames;
    };
    std::shared_ptr<Fields> f = std::make_shared<Fields>();

    auto run = [&app, ui, f]
    {
        int rank = toInt(f->rank, 3);
        EscalationResult result = f->start == 0
                                      ? app.session->escalate(app.currentCase(), rank)
                                      : app.session->escalateWork(app.currentCase(), f->workNames[f->start - 1], rank);
        ui->lastLadder = result;
        ui->hasLadder = true;
        app.say(result.message, result.handled);
    };

    Component start = Radiobox(&f->starts, &f->start);
    InputOption rankOption = Theme::input();
    rankOption.on_enter = run;
    Component rank = Input(&f->rank, "3", rankOption);
    Component go = Button("Escalate", run, Theme::button());
    Component close = Button("Close", [ui] { ui->escalate = false; }, Theme::button());
    Component form = Container::Vertical({start, rank, Container::Horizontal({go, close})});

    Component view = Renderer(form, [ui, start, rank, go, close]
    {
        Element content = vbox({
            paragraph("A request that needs a rank goes to the first in line. Anyone whose rank is too low "
                      "passes it to the next one (Chain of Responsibility).") | dim,
            text(""),
            row("Start with", start->Render()),
            row("Rank needed", rank->Render() | size(WIDTH, EQUAL, 6)),
            text(""),
            hbox({go->Render(), text("  "), close->Render(), filler(), text("esc closes") | dim}),
            separatorLight(),
            ui->hasLadder ? Ladder(ui->lastLadder) : text("The ladder shows here after you escalate.") | dim,
        });
        return frame("Escalate", content, 78);
    });

    auto onOpen = [&app, ui, f]
    {
        f->starts.clear();
        f->workNames.clear();
        std::vector<ChainStep> chain = app.session->chain();
        f->starts.push_back("The escalation chain" + (chain.empty() ? std::string("") : " (" + chain.front().name + " first)"));
        for (const StageView &stage : app.session->stages(app.currentCase()))
        {
            for (const WorkView &work : stage.work)
            {
                if (work.added && !work.person.empty())
                {
                    f->starts.push_back("\"" + work.name + "\" (" + work.person + " first)");
                    f->workNames.push_back(work.name);
                }
            }
        }
        f->start = 0;
        f->rank.clear();
        ui->hasLadder = false;
    };
    return {view, onOpen};
}
