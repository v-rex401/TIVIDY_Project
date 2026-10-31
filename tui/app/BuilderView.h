/**
 * @file BuilderView.h
 * @brief The "Build my own" screen: people, stages and routing, typed in, then run.
 * @author JP
 * @date 2026-10-31
 */

#ifndef BUILDERVIEW_H
#define BUILDERVIEW_H

#include <ftxui/component/component.hpp>

#include "AppState.h"

/**
 * @brief Make the build-your-own screen.
 *
 * It fills app.draft, the same ScenarioSetup a ready-made scenario fills,
 * then starts a session from it exactly like the ready-made ones.
 *
 * @param app The shared state.
 * @return The component.
 */
ftxui::Component BuilderView(AppState &app);

#endif
