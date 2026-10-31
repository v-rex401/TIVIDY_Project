/**
 * @file AddPersonForm.cpp
 * @brief The "Add a person" pop-up.
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

Popup AddPersonForm(AppState &app, std::shared_ptr<MainUi> ui)
{
    /** @brief The form's fields. */
    struct Fields
    {
        std::string name;
        std::string title;
        std::string rank = "1";
        int team = 0;
        std::vector<std::string> teams;
        std::string last;
    };
    std::shared_ptr<Fields> f = std::make_shared<Fields>();

    auto add = [&app, f]
    {
        std::string team = f->team == 0 ? "" : f->teams[f->team];
        ActionResult result = app.session->addPerson({f->name, f->title, toInt(f->rank, 1), team});
        app.say(result.message, result.changed);
        f->last = result.message;
        if (result.changed)
        {
            f->name.clear(); // keep title, rank and team: the next person is often similar
        }
    };

    InputOption option = Theme::input();
    option.on_enter = add;
    Component name = Input(&f->name, "full name", option);
    Component title = Input(&f->title, "job title, e.g. Nurse", option);
    Component rank = Input(&f->rank, "1-9", option);
    Component team = Toggle(&f->teams, &f->team);
    Component ok = Button("Add", add, Theme::button());
    Component done = Button("Done", [ui] { ui->addPerson = false; }, Theme::button());
    Component form = Container::Vertical({name, title, rank, team, Container::Horizontal({ok, done})});

    Component view = Renderer(form, [f, name, title, rank, team, ok, done]
    {
        Element content = vbox({
            paragraph("Enter in any field adds the person. The form stays open so you can add the next one.") | dim,
            text(""),
            row("Name", name->Render()),
            row("Title", title->Render()),
            row("Rank", rank->Render() | size(WIDTH, EQUAL, 6)),
            row("Team", team->Render()),
            text(""),
            hbox({ok->Render(), text("  "), done->Render(), filler(), text("esc closes") | dim}),
            f->last.empty() ? text("") : text(f->last) | color(Theme::accent()),
        });
        return frame("Add a person", content, 72);
    });

    auto onOpen = [&app, f]
    {
        f->teams = {"no team"};
        for (const TeamSpec &t : app.session->setup().teams)
        {
            f->teams.push_back(t.name);
        }
        f->team = 0;
        f->name.clear();
        f->last.clear();
    };
    return {view, onOpen};
}
