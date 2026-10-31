/**
 * @file AppState.cpp
 * @brief Implementation of the shared TUI state.
 * @author JP
 * @date 2026-10-31
 */

#include "AppState.h"

WorkflowInstance *AppState::currentCase()
{
    if (!session)
    {
        return nullptr;
    }
    std::vector<WorkflowInstance *> all = session->cases();
    if (all.empty())
    {
        return nullptr;
    }
    if (caseIndex < 0 || caseIndex >= static_cast<int>(all.size()))
    {
        caseIndex = static_cast<int>(all.size()) - 1;
    }
    return all[caseIndex];
}

void AppState::start(const ScenarioSetup &setup)
{
    session.reset(new Session(setup));
    session->openCase();
    caseIndex = 0;
    tab = 0;
    screen = MainScreen;
    say("Ready. The first case is open: try a for approve, w to add work, ? for help.", true);
}

void AppState::say(const std::string &message, bool ok)
{
    flash = message;
    flashOk = ok;
}
