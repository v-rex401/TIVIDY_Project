/**
 * @file PeopleView.h
 * @brief The People tab: the staff hierarchy and the escalation chain.
 * @author JP
 * @date 2026-10-31
 */

#ifndef PEOPLEVIEW_H
#define PEOPLEVIEW_H

#include <functional>
#include <memory>

#include <ftxui/component/component.hpp>

#include "AppState.h"
#include "MainUi.h"

/**
 * @brief Make the People tab.
 * @param app The shared state.
 * @param ui Pop-up state; the last escalation's ladder is drawn here too.
 * @param addPerson Opens the "Add a person" pop-up.
 * @param tryRequest Opens the "Escalate" pop-up.
 * @return The component.
 */
ftxui::Component PeopleView(AppState &app, std::shared_ptr<MainUi> ui, std::function<void()> addPerson,
                            std::function<void()> tryRequest);

#endif
