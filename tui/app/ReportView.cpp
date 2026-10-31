/**
 * @file ReportView.cpp
 * @brief The reports pop-up: visitor reports and saving the history.
 * @author JP
 * @date 2026-10-31
 */

#include "Forms.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include <ftxui/component/event.hpp>

#include "WorkflowInstance.h"
#include "FormParts.h"
#include "Theme.h"

using namespace ftxui;
using namespace FormParts;

Popup ReportView(AppState &app, std::shared_ptr<MainUi> ui)
{
    /** @brief The pop-up's state. */
    struct Fields
    {
        int which = 0;
        int scroll = 0;
        std::string content;
        std::vector<std::string> kinds = {"Progress report", "Audit trail"};
    };
    std::shared_ptr<Fields> f = std::make_shared<Fields>();

    auto refresh = [&app, f]
    {
        WorkflowInstance *c = app.currentCase();
        f->content = f->which == 0 ? app.session->progressReport(c) : app.session->auditTrail(c);
        f->scroll = 0;
    };

    Component kind = Theme::choice(&f->kinds, &f->which, refresh);

    Component body = Renderer([f](bool focused)
    {
        std::vector<std::string> lines;
        std::istringstream in(f->content);
        for (std::string line; std::getline(in, line);)
        {
            lines.push_back(line);
        }
        f->scroll = std::max(0, std::min(f->scroll, static_cast<int>(lines.size()) - 1));
        Elements rows;
        for (std::size_t i = f->scroll; i < lines.size() && rows.size() < 18; ++i)
        {
            rows.push_back(text(lines[i]));
        }
        Element box = vbox(rows) | size(HEIGHT, EQUAL, 18);
        return focused ? box | borderLight | color(Theme::accent()) : box | borderLight;
    });
    body = CatchEvent(body, [f](Event event)
    {
        if (event == Event::ArrowDown)
        {
            ++f->scroll;
            return true;
        }
        if (event == Event::ArrowUp)
        {
            --f->scroll;
            return true;
        }
        return false;
    });

    Component save = Button("Save history to CSV", [&app]
    {
        WorkflowInstance *c = app.currentCase();
        if (c == nullptr)
        {
            return;
        }
        ActionResult result = app.session->saveHistory(c, "history-" + c->getId() + ".csv");
        app.say(result.message, result.changed);
    }, Theme::button());
    Component close = Button("Close", [ui] { ui->report = false; }, Theme::button());
    Component form = Container::Vertical({kind, body, Container::Horizontal({save, close})});

    Component view = Renderer(form, [&app, kind, body, save, close]
    {
        WorkflowInstance *c = app.currentCase();
        Element content = vbox({
            kind->Render(),
            paragraph("A visitor walks the case's stages, work, people and history and builds this report, "
                      "without the case's classes knowing about reports.") | dim,
            body->Render(),
            hbox({save->Render(), text("  "), close->Render(), filler(), text("↑↓ scroll   esc closes") | dim}),
        });
        return frame("Reports for " + (c != nullptr ? c->getId() : std::string("?")), content, 84);
    });

    auto onOpen = [f, refresh]
    {
        f->which = 0;
        refresh();
    };
    return {view, onOpen};
}
