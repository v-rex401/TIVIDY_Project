/**
 * @file Forms.h
 * @brief The main screen's pop-ups: add work, escalate, change a value, reports, help, add a person.
 * @author JP
 * @date 2026-10-31
 */

#ifndef FORMS_H
#define FORMS_H

#include <functional>
#include <memory>

#include <ftxui/component/component.hpp>

#include "AppState.h"
#include "MainUi.h"

/** @brief A pop-up and what to do each time it opens (reset its fields, refresh its lists). */
struct Popup
{
    ftxui::Component component;  ///< What is drawn and receives keys.
    std::function<void()> onOpen; ///< Call right before showing it.
};

/**
 * @brief "Add work": quick picks, or type a task, then choose assignment and routing.
 * @param app The shared state.
 * @param ui Pop-up state.
 * @return The pop-up.
 */
Popup AddWorkForm(AppState &app, std::shared_ptr<MainUi> ui);

/**
 * @brief "Escalate": send a request needing a rank up the staff chain and draw the ladder.
 * @param app The shared state.
 * @param ui Pop-up state.
 * @return The pop-up.
 */
Popup EscalateForm(AppState &app, std::shared_ptr<MainUi> ui);

/**
 * @brief "Change a value": set one of the case's values (what the routing reads).
 * @param app The shared state.
 * @param ui Pop-up state.
 * @return The pop-up.
 */
Popup ValueForm(AppState &app, std::shared_ptr<MainUi> ui);

/**
 * @brief Reports from the visitors, and saving the history.
 * @param app The shared state.
 * @param ui Pop-up state.
 * @return The pop-up.
 */
Popup ReportView(AppState &app, std::shared_ptr<MainUi> ui);

/**
 * @brief What every key does, in plain words, with the pattern behind it.
 * @param ui Pop-up state.
 * @return The pop-up.
 */
Popup HelpView(std::shared_ptr<MainUi> ui);

/**
 * @brief "Add a person": name, title, rank and team; stays open to add several quickly.
 * @param app The shared state.
 * @param ui Pop-up state.
 * @return The pop-up.
 */
Popup AddPersonForm(AppState &app, std::shared_ptr<MainUi> ui);

/**
 * @brief Draw an escalation as a ladder: each person, their rank, and whether they took it.
 * @param result The escalation.
 * @return The element.
 */
ftxui::Element Ladder(const EscalationResult &result);

#endif
