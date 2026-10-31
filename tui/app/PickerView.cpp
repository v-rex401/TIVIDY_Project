/**
 * @file PickerView.cpp
 * @brief Implementation of the picker screen.
 * @author JP
 * @date 2026-10-31
 */

#include "PickerView.h"

#include <memory>

#include <ftxui/component/event.hpp>

#include "Theme.h"

using namespace ftxui;

Component PickerView(AppState &app)
{
    std::shared_ptr<int> selected = std::make_shared<int>(0);

    // The ready-made scenarios first, then "build my own" last.
    auto count = [&app] { return static_cast<int>(app.readyMade.size()) + 1; };
    auto choose = [&app, selected]
    {
        if (*selected < static_cast<int>(app.readyMade.size()))
        {
            app.start(app.readyMade[*selected]);
        }
        else
        {
            app.screen = BuilderScreen;
        }
    };

    Component view = Renderer([&app, selected, count](bool)
    {
        Elements rows;
        for (int i = 0; i < count(); ++i)
        {
            bool ready = i < static_cast<int>(app.readyMade.size());
            std::string name = ready ? app.readyMade[i].name : "Build my own";
            std::string tag = ready ? "ready-made" : "you set it up";
            std::string summary = ready ? app.readyMade[i].summary
                                        : "Add your own people, stages and routing, then run it the same way.";
            bool here = i == *selected;

            Element title = text(name) | bold;
            if (here)
            {
                title = title | color(Theme::accent());
            }
            rows.push_back(hbox({text(here ? " › " : "   ") | color(Theme::accent()), title, filler(),
                                 text(tag + " ") | dim}));
            rows.push_back(hbox({text("   "), paragraph(summary) | dim | size(WIDTH, LESS_THAN, 66)}));
            rows.push_back(text(""));
        }

        Element body = vbox({
            text(""),
            hbox({text("TIVIDY") | bold | color(Theme::accent()), text("   a workflow engine you can watch work") | dim}),
            text(""),
            text("How do you want to start?"),
            text(""),
            vbox(rows) | size(WIDTH, EQUAL, 72),
            text(""),
            hbox({Theme::keyHint("↑↓", "choose"), Theme::keyHint("enter", "start"), Theme::keyHint("q", "quit")}),
        });
        return body | center | flex;
    });

    return CatchEvent(view, [&app, selected, count, choose](Event event)
    {
        if (event == Event::ArrowUp || event == Event::Character('k'))
        {
            *selected = (*selected + count() - 1) % count();
            return true;
        }
        if (event == Event::ArrowDown || event == Event::Character('j'))
        {
            *selected = (*selected + 1) % count();
            return true;
        }
        if (event == Event::Return)
        {
            choose();
            return true;
        }
        if (event.is_character() && event.character()[0] >= '1' && event.character()[0] <= '9')
        {
            int number = event.character()[0] - '1';
            if (number < count())
            {
                *selected = number;
                choose();
            }
            return true;
        }
        if (event == Event::Character('q') && app.quit)
        {
            app.quit();
            return true;
        }
        return false;
    });
}
