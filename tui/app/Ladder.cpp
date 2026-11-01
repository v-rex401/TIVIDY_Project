/**
 * @file Ladder.cpp
 * @brief Draws an escalation as a ladder.
 * @author JP
 * @date 2026-10-31
 */

#include "Forms.h"

#include <string>

#include "FormParts.h"
#include "Theme.h"

using namespace ftxui;
using namespace FormParts;

Element Ladder(const EscalationResult &result)
{
    Elements rows;
    rows.push_back(text("needs rank " + std::to_string(result.rank) +
                        (result.startedWith.empty() ? "" : ", starting with " + result.startedWith)) | dim);
    for (const ChainStep &step : result.steps)
    {
        Element verdict = step.canHandle ? text("takes it ✓") | color(Theme::stateColour("Active")) | bold
                                         : text("passes it on") | dim;
        rows.push_back(hbox({
            text(step.canHandle ? " ▲ " : " │ ") | color(Theme::accent()),
            text(step.name) | size(WIDTH, EQUAL, 22),
            text(step.title) | dim | size(WIDTH, EQUAL, 18),
            text("rank " + std::to_string(step.rank)) | size(WIDTH, EQUAL, 9),
            verdict,
        }));
    }
    rows.push_back(text(""));
    rows.push_back(result.handled
                       ? text("handleRequest() said yes: someone in the chain can approve it") | color(Theme::accent())
                       : text("handleRequest() said no: nobody in the chain has that rank") | color(Theme::refused()));
    return vbox(rows);
}
