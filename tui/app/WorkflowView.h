/**
 * @file WorkflowView.h
 * @brief The Workflow tab: the definition the Builder made, and the strategies it runs with.
 * @author JP
 * @date 2026-10-31
 */

#ifndef WORKFLOWVIEW_H
#define WORKFLOWVIEW_H

#include <ftxui/component/component.hpp>

#include "AppState.h"

/**
 * @brief Make the Workflow tab.
 * @param app The shared state.
 * @return The component.
 */
ftxui::Component WorkflowView(AppState &app);

#endif
