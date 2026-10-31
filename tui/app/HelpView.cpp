/**
 * @file HelpView.cpp
 * @brief The help pop-up: every key, in plain words, with its pattern.
 * @author JP
 * @date 2026-10-31
 */

#include "Forms.h"

#include <string>

#include "FormParts.h"
#include "Theme.h"

using namespace ftxui;
using namespace FormParts;

Popup HelpView(std::shared_ptr<MainUi> ui)
{
    Component close = Button("Got it", [ui] { ui->help = false; }, Theme::button());

    Component view = Renderer(close, [close]
    {
        auto line = [](const std::string &key, const std::string &what, const std::string &pattern)
        {
            return hbox({text(" " + key) | bold | color(Theme::accent()) | size(WIDTH, EQUAL, 6),
                         text(what) | size(WIDTH, EQUAL, 56), text(pattern) | dim});
        };
        Element content = vbox({
            text("On a case") | bold,
            line("a", "Approve: move the case on one state", "Command, State"),
            line("r", "Reject: send it back to Pending", "Command, State"),
            line("t", "Retry: back to Pending, then straight on to Active", "Command, State"),
            line("c", "Cancel: stop the case", "Command, State"),
            line("u", "Undo the last of those, from the snapshot taken first", "Memento"),
            line("x", "Execute: the facade runs the case on (no undo)", "Facade"),
            line("w", "Add work: someone is picked, then a stage", "Strategy, Adapter"),
            line("e", "Escalate: pass a request up until someone has the rank", "Chain of Resp."),
            line("v", "Change a value the routing reads, on this case only", "Prototype"),
            line("p", "Reports on the case, and save its history", "Visitor"),
            line("n", "New case from the same definition", "Facade, Prototype"),
            text(""),
            text("Anywhere") | bold,
            line("1-4", "Cases, Workflow, People, Log tabs", ""),
            line("↑↓ ←→", "Move between the case list and the buttons", ""),
            line("s", "Switch scenario (back to the start screen)", ""),
            line("q", "Quit", ""),
            text(""),
            paragraph("Every change of state is announced to two observers: the audit log keeps it, the progress "
                      "monitor prints it in the engine strip at the bottom (Observer).") | dim,
            text(""),
            close->Render(),
        });
        return frame("What the keys do", content, 92);
    });
    return {view, [] {}};
}
