/**
 * @file LogView.cpp
 * @brief Implementation of the Log tab.
 * @author JP
 * @date 2026-10-31
 */

#include "LogView.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/event.hpp>

#include "Theme.h"

using namespace ftxui;

namespace
{
    /**
     * @brief Mark the row that should be in view, counting back from the newest.
     * @param rows The rows, oldest first.
     * @param back How many rows up from the newest.
     * @return The rows inside a scrolling frame.
     */
    Element scrolled(Elements rows, int back)
    {
        if (!rows.empty())
        {
            int at = std::max(0, static_cast<int>(rows.size()) - 1 - back);
            rows[at] = rows[at] | focus;
        }
        return vbox(std::move(rows)) | yframe | flex;
    }
}

Component LogView(AppState &app)
{
    std::shared_ptr<int> back = std::make_shared<int>(0);

    Component view = Renderer([&app, back](bool)
    {
        if (!app.session)
        {
            return text("");
        }
        Session &session = *app.session;

        Elements story;
        for (const TimelineEntry &entry : session.timeline())
        {
            Element line = text((entry.refused ? "✗ " : "") + entry.text);
            if (entry.refused)
            {
                line = line | color(Theme::refused());
            }
            story.push_back(hbox({text(entry.time + "  ") | dim,
                                  text((entry.caseId.empty() ? "     " : entry.caseId) + "  ") | bold, line | flex,
                                  text("  " + entry.pattern) | dim}));
        }

        Elements engine;
        for (const std::string &line : session.engineLog())
        {
            engine.push_back(text(line) | dim);
        }
        if (engine.empty())
        {
            engine.push_back(text("Nothing printed yet.") | dim);
        }

        int limit = std::max(0, static_cast<int>(story.size()) - 1);
        *back = std::min(*back, limit);
        return vbox({
            Theme::panel("Everything that happened, all cases (↑↓ to scroll)", scrolled(std::move(story), *back)) | flex,
            Theme::panel("What the engine printed (states, observers, notifications)", scrolled(std::move(engine), 0)) |
                size(HEIGHT, EQUAL, 12),
        });
    });

    return CatchEvent(view, [back](Event event)
    {
        if (event == Event::ArrowUp || event == Event::PageUp)
        {
            *back += event == Event::PageUp ? 10 : 1;
            return true;
        }
        if (event == Event::ArrowDown || event == Event::PageDown)
        {
            *back = std::max(0, *back - (event == Event::PageDown ? 10 : 1));
            return true;
        }
        return false;
    });
}
