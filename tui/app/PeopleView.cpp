/**
 * @file PeopleView.cpp
 * @brief Implementation of the People tab.
 * @author JP
 * @date 2026-10-31
 */

#include "PeopleView.h"

#include <string>
#include <vector>

#include "Forms.h"
#include "Staff.h"
#include "Theme.h"

using namespace ftxui;

Component PeopleView(AppState &app, std::shared_ptr<MainUi> ui, std::function<void()> addPerson,
                     std::function<void()> tryRequest)
{
    Component add = Button("a  Add a person", std::move(addPerson), Theme::button());
    Component request = Button("e  Try a request", std::move(tryRequest), Theme::button());
    Component buttons = Container::Horizontal({add, request});

    return Renderer(buttons, [&app, ui, add, request]
    {
        if (!app.session)
        {
            return text("");
        }
        Session &session = *app.session;

        // The engine draws the tree itself (printHierarchy); only the styling is added here.
        Elements tree;
        for (const std::string &line : session.staffTree())
        {
            std::size_t indent = line.find_first_not_of(' ');
            std::string body = line.substr(indent == std::string::npos ? 0 : indent);
            bool team = body.rfind("Team: ", 0) == 0;
            std::string name = body.substr(body.find(": ") + 2);
            Element row = hbox({text(std::string(indent == std::string::npos ? 0 : indent, ' ')),
                                text(team ? "▸ " : "· ") | dim, team ? text(name) | bold : text(name)});
            tree.push_back(row);
        }

        Elements chain;
        std::vector<ChainStep> steps = session.chain();
        for (std::size_t i = 0; i < steps.size(); ++i)
        {
            chain.push_back(hbox({
                text(" " + std::to_string(i + 1) + " ") | color(Theme::accent()) | bold,
                text(steps[i].name) | size(WIDTH, EQUAL, 22),
                text(steps[i].title) | dim | size(WIDTH, EQUAL, 18),
                text("rank " + std::to_string(steps[i].rank)),
            }));
            if (i + 1 < steps.size())
            {
                chain.push_back(text("   ↓ passes on what it can't approve") | dim);
            }
        }
        if (chain.empty())
        {
            chain.push_back(text("No escalation chain set up.") | dim);
        }

        Elements roster;
        for (Staff *person : session.roster())
        {
            roster.push_back(hbox({text(person->getName()) | size(WIDTH, EQUAL, 22),
                                   text(person->getTitle()) | dim | size(WIDTH, EQUAL, 18),
                                   text(std::to_string(person->getRank()))}));
        }

        Element right = vbox({
            Theme::panel("Escalation chain (Chain of Responsibility)", vbox(chain)),
            Theme::panel("Last request", ui->hasLadder ? Ladder(ui->lastLadder)
                                                       : text("Press e to send a request up the chain.") | dim),
        });

        return vbox({
            hbox({
                Theme::panel("Staff (Composite)", vbox(tree) | yframe) |
                    size(WIDTH, EQUAL, 46),
                vbox({
                    right,
                    Theme::panel("Everyone who can be given work", vbox(roster) | yframe) | flex,
                }) | flex,
            }) | flex,
            hbox({add->Render(), text("  "), request->Render()}),
        });
    });
}
