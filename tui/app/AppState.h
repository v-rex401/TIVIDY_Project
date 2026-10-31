/**
 * @file AppState.h
 * @brief What the TUI screens share: the running session and where the user is.
 * @author JP
 * @date 2026-10-31
 */

#ifndef APPSTATE_H
#define APPSTATE_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ScenarioSetup.h"
#include "Session.h"

class WorkflowInstance;

/** @brief The three screens, in the order of the root tab container. */
enum AppScreen
{
    PickerScreen = 0,  ///< Choose a ready-made scenario or build your own.
    BuilderScreen = 1, ///< Build your own setup.
    MainScreen = 2,    ///< Run workflows.
};

/**
 * @brief State shared by every screen of the TUI.
 *
 * The screens only read the Session and call its methods; they keep their
 * own form fields. This struct ties them together.
 */
struct AppState
{
    std::vector<ScenarioSetup> readyMade; ///< Scenarios offered on the picker.
    ScenarioSetup draft;                  ///< The build-your-own setup being edited.
    std::unique_ptr<Session> session;     ///< The running session, once one is started.

    int screen = PickerScreen; ///< Which screen is showing.
    int tab = 0;               ///< Which tab of the main screen is showing.
    int caseIndex = 0;         ///< Selected case in the case list.

    std::string flash;     ///< Result of the last action, shown above the key bar.
    bool flashOk = true;   ///< False when the engine refused the last action.

    std::function<void()> quit; ///< Ends the program.

    /** @return The selected case, or nullptr if there are none. */
    WorkflowInstance *currentCase();

    /**
     * @brief Start a session from a setup, open its first case and show the main screen.
     * @param setup The setup to run.
     */
    void start(const ScenarioSetup &setup);

    /**
     * @brief Show the result of an action above the key bar.
     * @param message What happened.
     * @param ok False if the engine refused.
     */
    void say(const std::string &message, bool ok);
};

#endif
