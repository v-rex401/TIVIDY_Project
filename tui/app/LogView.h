/**
 * @file LogView.h
 * @brief The Log tab: every timeline entry across all cases, and everything the engine printed.
 * @author JP
 * @date 2026-10-31
 */

#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <ftxui/component/component.hpp>

#include "AppState.h"

/**
 * @brief Make the Log tab.
 * @param app The shared state.
 * @return The component.
 */
ftxui::Component LogView(AppState &app);

#endif
