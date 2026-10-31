/**
 * @file PickerView.h
 * @brief The first screen: pick a ready-made scenario or build your own.
 * @author JP
 * @date 2026-10-31
 */

#ifndef PICKERVIEW_H
#define PICKERVIEW_H

#include <ftxui/component/component.hpp>

#include "AppState.h"

/**
 * @brief Make the picker screen.
 * @param app The shared state; picking starts a session or opens the builder.
 * @return The component.
 */
ftxui::Component PickerView(AppState &app);

#endif
