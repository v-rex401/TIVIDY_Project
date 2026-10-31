/**
 * @file CasesView.h
 * @brief The Cases tab: pick a case, see where it is, and act on it.
 * @author JP
 * @date 2026-10-31
 */

#ifndef CASESVIEW_H
#define CASESVIEW_H

#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>

#include "AppState.h"
#include "MainUi.h"

/**
 * @brief Draw the four states as a route, with the path a case actually took in bold.
 * @param path The states the case went through, oldest first.
 * @return The element.
 */
ftxui::Element StateTrack(const std::vector<std::string> &path);

/**
 * @brief Make the Cases tab.
 * @param app The shared state.
 * @param ui Pop-up state; the board highlights ui->lastWork.
 * @param actions What each button does.
 * @return The component.
 */
ftxui::Component CasesView(AppState &app, std::shared_ptr<MainUi> ui, const CaseActions &actions);

#endif
