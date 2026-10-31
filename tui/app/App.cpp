/**
 * @file App.cpp
 * @brief Implementation of the TUI entry point.
 * @author JP
 * @date 2026-10-31
 */

#include "App.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "AppState.h"
#include "BuilderView.h"
#include "MainView.h"
#include "PickerView.h"
#include "Scenarios.h"

using namespace ftxui;

int App::run()
{
    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    AppState app;
    app.readyMade = Scenarios::all();
    app.quit = screen.ExitLoopClosure();

    Component root = Container::Tab({PickerView(app), BuilderView(app), MainView(app)}, &app.screen);
    screen.Loop(root);
    return 0;
}
