/**
 * @file MainView.h
 * @brief The main screen: title bar, the four tabs, the engine strip, the key bar and the pop-ups.
 * @author JP
 * @date 2026-10-31
 */

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <ftxui/component/component.hpp>

#include "AppState.h"

/**
 * @brief Make the main screen.
 * @param app The shared state; it must have a session before this screen is shown.
 * @return The component.
 */
ftxui::Component MainView(AppState &app);

#endif
