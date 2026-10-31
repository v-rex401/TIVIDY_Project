/**
 * @file MainView.cpp
 * @brief Implementation of the main screen.
 * @author JP
 * @date 2026-10-31
 */

#include "MainView.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/event.hpp>

#include "CasesView.h"
#include "Forms.h"
#include "LogView.h"
#include "PeopleView.h"
#include "Theme.h"
#include "WorkflowInstance.h"
#include "WorkflowView.h"

using namespace ftxui;

namespace
{
    const std::vector<std::string> kTabs = {"Cases", "Workflow", "People", "Log"}; ///< Tab names, keys 1 to 4.

    /**
     * @brief The key bar for a tab.
     * @param tab The tab showing.
     * @return The element.
     */
    Element keyBar(int tab)
    {
        Elements keys;
        if (tab == 0)
        {
            keys = {Theme::keyHint("a", "approve"), Theme::keyHint("r", "reject"), Theme::keyHint("t", "retry"),
                    Theme::keyHint("c", "cancel"), Theme::keyHint("u", "undo"), Theme::keyHint("w", "add work"),
                    Theme::keyHint("e", "escalate"), Theme::keyHint("n", "new case")};
        }
        else if (tab == 2)
        {
            keys = {Theme::keyHint("a", "add a person"), Theme::keyHint("e", "try a request")};
        }
        else if (tab == 3)
        {
            keys = {Theme::keyHint("↑↓", "scroll")};
        }
        keys.push_back(filler());
        keys.push_back(Theme::keyHint("1-4", "tabs"));
        keys.push_back(Theme::keyHint("?", "help"));
        keys.push_back(Theme::keyHint("s", "switch scenario"));
        keys.push_back(Theme::keyHint("q", "quit"));
        return hbox(keys);
    }
}

Component MainView(AppState &app)
{
    std::shared_ptr<MainUi> ui = std::make_shared<MainUi>();

    Popup addWork = AddWorkForm(app, ui);
    Popup escalate = EscalateForm(app, ui);
    Popup value = ValueForm(app, ui);
    Popup report = ReportView(app, ui);
    Popup help = HelpView(ui);
    Popup addPerson = AddPersonForm(app, ui);

    // Open a pop-up: refresh it, then show it.
    auto open = [ui](bool MainUi::*flag, std::function<void()> refresh)
    {
        return [ui, flag, refresh]
        {
            ui->closeAll();
            refresh();
            (*ui).*flag = true;
        };
    };
    auto result = [&app](const ActionResult &r) { app.say(r.message, r.changed); };

    CaseActions actions;
    actions.approve = [&app, result] { result(app.session->approve(app.currentCase())); };
    actions.reject = [&app, result] { result(app.session->reject(app.currentCase())); };
    actions.retry = [&app, result] { result(app.session->retry(app.currentCase())); };
    actions.cancel = [&app, result] { result(app.session->cancel(app.currentCase())); };
    actions.undo = [&app, result] { result(app.session->undo(app.currentCase())); };
    actions.execute = [&app, result] { result(app.session->execute(app.currentCase())); };
    actions.addWork = open(&MainUi::addWork, addWork.onOpen);
    actions.escalate = open(&MainUi::escalate, escalate.onOpen);
    actions.editValue = open(&MainUi::value, value.onOpen);
    actions.reports = open(&MainUi::report, report.onOpen);
    actions.newCase = [&app]
    {
        WorkflowInstance *c = app.session->openCase();
        app.caseIndex = static_cast<int>(app.session->cases().size()) - 1;
        app.say(c->getId() + " opened. It has its own copy of every stage, so nothing you do to it touches the others.",
                true);
    };
    std::function<void()> openHelp = open(&MainUi::help, help.onOpen);
    std::function<void()> openAddPerson = open(&MainUi::addPerson, addPerson.onOpen);

    Component tabs = Container::Tab({
        CasesView(app, ui, actions),
        WorkflowView(app),
        PeopleView(app, ui, openAddPerson, actions.escalate),
        LogView(app),
    }, &app.tab);

    Component screen = Renderer(tabs, [&app, tabs]
    {
        if (!app.session)
        {
            return text("");
        }
        Elements tabLabels;
        for (std::size_t i = 0; i < kTabs.size(); ++i)
        {
            Element label = text(" " + std::to_string(i + 1) + " " + kTabs[i] + " ");
            tabLabels.push_back(static_cast<int>(i) == app.tab ? label | inverted | bold : label | dim);
        }
        Element title = hbox({
            text(" TIVIDY ") | bold | color(Theme::accent()),
            text(" " + app.session->setup().name + "   ") | bold,
            hbox(tabLabels),
            filler(),
            text("notifications sent " + std::to_string(app.session->notificationsSent()) + " ") | dim,
        });

        const std::vector<std::string> &printed = app.session->engineLog();
        Elements engine;
        for (std::size_t i = printed.size() > 2 ? printed.size() - 2 : 0; i < printed.size(); ++i)
        {
            engine.push_back(text(printed[i]));
        }
        while (engine.size() < 2)
        {
            engine.push_back(text(""));
        }

        Element flash = text(app.flash) | color(app.flashOk ? Theme::accent() : Theme::refused());
        return vbox({
            title,
            separatorLight(),
            tabs->Render() | flex,
            hbox({text(" engine › ") | dim, vbox(engine) | dim | flex}),
            separatorLight(),
            hbox({text(" "), flash}),
            hbox({text(" "), keyBar(app.tab)}),
        });
    });

    screen = screen | Modal(addWork.component, &ui->addWork) | Modal(escalate.component, &ui->escalate) |
             Modal(value.component, &ui->value) | Modal(report.component, &ui->report) |
             Modal(help.component, &ui->help) | Modal(addPerson.component, &ui->addPerson);

    std::map<char, std::function<void()>> caseKeys = {
        {'a', actions.approve}, {'r', actions.reject}, {'t', actions.retry}, {'c', actions.cancel},
        {'u', actions.undo}, {'x', actions.execute}, {'w', actions.addWork}, {'e', actions.escalate},
        {'v', actions.editValue}, {'p', actions.reports}, {'n', actions.newCase},
    };
    std::map<char, std::function<void()>> peopleKeys = {{'a', openAddPerson}, {'e', actions.escalate}};

    return CatchEvent(screen, [&app, ui, caseKeys, peopleKeys, openHelp](Event event)
    {
        if (!app.session)
        {
            return false;
        }
        if (ui->anyOpen())
        {
            if (event == Event::Escape)
            {
                ui->closeAll();
                return true;
            }
            return false; // the pop-up's fields get every other key
        }
        if (!event.is_character())
        {
            return false;
        }
        char key = event.character()[0];
        if (key >= '1' && key <= '4')
        {
            app.tab = key - '1';
            return true;
        }
        if (key == '?')
        {
            openHelp();
            return true;
        }
        if (key == 's')
        {
            app.screen = PickerScreen;
            return true;
        }
        if (key == 'q' && app.quit)
        {
            app.quit();
            return true;
        }
        const std::map<char, std::function<void()>> &keys = app.tab == 0 ? caseKeys : peopleKeys;
        std::map<char, std::function<void()>>::const_iterator found = keys.find(key);
        if ((app.tab == 0 || app.tab == 2) && found != keys.end())
        {
            found->second();
            return true;
        }
        return false;
    });
}
