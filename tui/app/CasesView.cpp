/**
 * @file CasesView.cpp
 * @brief Implementation of the Cases tab.
 * @author JP
 * @date 2026-10-31
 */

#include "CasesView.h"

#include <algorithm>
#include <set>
#include <sstream>
#include <utility>

#include "State.h"
#include "Theme.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"

using namespace ftxui;

namespace
{
    /**
     * @brief Whether the path moved between two states, in either direction.
     * @param path The states, oldest first.
     * @param a One state.
     * @param b The other state.
     * @return True if a step went a→b or b→a.
     */
    bool walked(const std::vector<std::string> &path, const std::string &a, const std::string &b)
    {
        for (std::size_t i = 1; i < path.size(); ++i)
        {
            if ((path[i - 1] == a && path[i] == b) || (path[i - 1] == b && path[i] == a))
            {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief A number without trailing zeros.
     * @param value The number.
     * @return The text.
     */
    std::string number(double value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }

    /**
     * @brief One action button: the key, then what it does.
     * @param key The shortcut.
     * @param label The action.
     * @param run What it does.
     * @return The button.
     */
    Component actionButton(const std::string &key, const std::string &label, std::function<void()> run)
    {
        ButtonOption option = Theme::button();
        option.transform = [key, label](const EntryState &state)
        {
            Element row = hbox({text(" " + key + " ") | bold | color(Theme::accent()), text(label + " "), filler()});
            return state.focused ? Theme::selected(row) : row;
        };
        return Button(label, std::move(run), option);
    }
}

Element StateTrack(const std::vector<std::string> &path)
{
    std::string current = path.empty() ? "Pending" : path.back();
    std::set<std::string> visited(path.begin(), path.end());

    auto node = [&](const std::string &state)
    {
        Element label = text(" " + state + " ");
        if (visited.count(state) != 0)
        {
            label = label | bold | color(Theme::stateColour(state));
        }
        else
        {
            label = label | dim;
        }
        return state == current ? Theme::selected(label | bold, Theme::stateColour(state)) : label;
    };
    auto edge = [&](const std::string &a, const std::string &b)
    {
        return walked(path, a, b) ? text("━━━━━") | color(Theme::accent()) : text("─────") | dim;
    };
    bool toCancelled = walked(path, "Active", "Cancelled") || walked(path, "Pending", "Cancelled");

    // " Pending " is 9 wide and an edge is 5, so Active's middle is column 18.
    Element top = hbox({node("Pending"), edge("Pending", "Active"), node("Active"), edge("Active", "Completed"),
                        node("Completed")});
    Element down = text(std::string(18, ' ') + (toCancelled ? "┃" : "│")) |
                   (toCancelled ? color(Theme::accent()) : dim);
    Element bottom = hbox({text(std::string(13, ' ')), node("Cancelled")});

    // The path in words, so going back and forth is visible too.
    std::string words;
    std::size_t from = path.size() > 9 ? path.size() - 9 : 0;
    for (std::size_t i = from; i < path.size(); ++i)
    {
        words += (i == from ? (from > 0 ? "… " : "") : " → ") + path[i];
    }

    return vbox({top, down, bottom, text(""), hbox({text("path  ") | dim, text(words)})});
}

Component CasesView(AppState &app, std::shared_ptr<MainUi> ui, const CaseActions &actions)
{
    // Case list: "WF-1|Active" per entry, drawn as id plus a coloured state.
    std::shared_ptr<std::vector<std::string>> labels = std::make_shared<std::vector<std::string>>();
    MenuOption menuOption = MenuOption::Vertical();
    menuOption.entries_option.transform = [](const EntryState &state)
    {
        std::size_t bar = state.label.find('|');
        std::string id = state.label.substr(0, bar);
        std::string status = bar == std::string::npos ? "" : state.label.substr(bar + 1);
        Element row = hbox({text(state.active ? " › " : "   ") | color(Theme::accent()), text(id) | bold,
                            text("  "), Theme::stateLabel(status), filler()});
        return state.focused ? Theme::selected(row) : row;
    };
    Component caseList = Menu(labels.get(), &app.caseIndex, menuOption);

    Component buttons = Container::Vertical({
        actionButton("a", "Approve", actions.approve),
        actionButton("r", "Reject", actions.reject),
        actionButton("t", "Retry", actions.retry),
        actionButton("c", "Cancel", actions.cancel),
        actionButton("u", "Undo last", actions.undo),
        actionButton("x", "Execute", actions.execute),
        actionButton("w", "Add work", actions.addWork),
        actionButton("e", "Escalate", actions.escalate),
        actionButton("v", "Change a value", actions.editValue),
        actionButton("p", "Reports", actions.reports),
        actionButton("n", "New case", actions.newCase),
    });

    Component layout = Container::Horizontal({caseList, buttons});

    return Renderer(layout, [&app, ui, labels, caseList, buttons]
    {
        Session &session = *app.session;
        labels->clear();
        for (WorkflowInstance *c : session.cases())
        {
            labels->push_back(c->getId() + "|" + c->getState()->getName());
        }
        WorkflowInstance *c = app.currentCase();
        if (c == nullptr)
        {
            return text("No cases yet. Press n to open one.") | center;
        }

        // ----- left: case list -----
        Element left = Theme::panel("Cases", vbox({
            caseList->Render() | yframe | flex,
            separatorLight(),
            text(" n  new case") | dim,
        })) | size(WIDTH, EQUAL, 22);

        // ----- middle: where the case is -----
        Element header = hbox({
            text(c->getId()) | bold,
            text("  " + session.definition()->getName()) | dim,
            filler(),
            text("now "), Theme::stateLabel(c->getState()->getName()),
        });

        Elements stageRows;
        for (const StageView &stage : session.stages(c))
        {
            stageRows.push_back(hbox({text(stage.name) | bold, text("  " + std::to_string(stage.work.size())) | dim}));
            for (const WorkView &work : stage.work)
            {
                if (!work.added)
                {
                    stageRows.push_back(text("   " + work.name) | dim);
                    continue;
                }
                Element line = hbox({
                    text(" + " + work.name),
                    text("  " + work.person + (work.role.empty() ? "" : " (" + work.role + ")")) | dim,
                    work.severity.empty() ? text("") : text("  " + work.severity) | dim,
                });
                stageRows.push_back(work.name == ui->lastWork ? line | color(Theme::accent()) | bold : line);
            }
        }
        Element board = Theme::panel("Stages and work", vbox(stageRows) | yframe | flex);

        Elements valueRows;
        for (const std::string &key : session.dataKeys())
        {
            valueRows.push_back(hbox({text(key), filler(), text(number(c->getData(key))) | bold}));
        }
        if (valueRows.empty())
        {
            valueRows.push_back(text("none") | dim);
        }
        valueRows.push_back(separatorLight());
        valueRows.push_back(hbox({text("can undo"), filler(), text(std::to_string(session.undoDepth(c))) | bold}));
        valueRows.push_back(hbox({text("notifications"), filler(), text(std::to_string(session.notificationsSent())) | bold}));
        valueRows.push_back(hbox({text("people on it"), filler(), text(std::to_string(c->getParticipants().size())) | bold}));
        Element values = Theme::panel("Case values", vbox(valueRows)) | size(WIDTH, EQUAL, 30);

        std::vector<TimelineEntry> story = session.timeline(c);
        Elements storyRows;
        for (std::size_t i = 0; i < story.size(); ++i)
        {
            const TimelineEntry &entry = story[i];
            bool newest = i + 1 == story.size();
            Element line = paragraph((entry.refused ? "✗ " : "") + entry.text);
            if (entry.refused)
            {
                line = line | color(Theme::refused());
            }
            else if (newest)
            {
                line = line | color(Theme::accent()) | bold;
            }
            Element row = hbox({text(entry.time + "  ") | dim, line | flex, text("  " + entry.pattern) | dim});
            storyRows.push_back(newest ? row | focus : row);
        }
        Element timeline = Theme::panel("What happened", vbox(storyRows) | yframe | flex);

        Element middle = vbox({
            Theme::panel("Status", vbox({header, text(""), StateTrack(session.statePath(c))})),
            hbox({board | flex, values}) | flex,
            timeline | size(HEIGHT, EQUAL, 10),
        }) | flex;

        // ----- right: actions -----
        Element right = Theme::panel("Do something", vbox({
            buttons->Render(),
            separatorLight(),
            paragraph("Click, or press the key. ? explains each one.") | dim,
        })) | size(WIDTH, EQUAL, 24);

        return hbox({left, middle, right});
    });
}
