/**
 * @file FormParts.h
 * @brief Small pieces shared by the pop-up forms.
 * @author JP
 * @date 2026-10-31
 */

#ifndef FORMPARTS_H
#define FORMPARTS_H

#include <string>
#include <vector>

#include <ftxui/dom/elements.hpp>

#include "ScenarioSetup.h"

class WorkflowInstance;

/** @brief Helpers for the pop-ups in Forms.h. */
namespace FormParts
{
    /** @brief Assignment strategy names, in the order the forms list them. */
    extern const std::vector<std::string> kAssignmentKinds;

    /** @brief Routing strategy names, in the order the forms list them. */
    extern const std::vector<std::string> kRoutingKinds;

    /**
     * @brief A number without trailing zeros.
     * @param value The number.
     * @return The text.
     */
    std::string number(double value);

    /**
     * @brief Read a whole number, falling back when the text isn't one.
     * @param text The text.
     * @param fallback Used if it doesn't parse.
     * @return The number.
     */
    int toInt(const std::string &text, int fallback);

    /**
     * @brief Position of a name in a list.
     * @param list The list.
     * @param name The name.
     * @return Its index, or 0 if it isn't there.
     */
    int indexOf(const std::vector<std::string> &list, const std::string &name);

    /**
     * @brief A labelled form row.
     * @param label The label on the left.
     * @param field The field.
     * @return The row.
     */
    ftxui::Element row(const std::string &label, ftxui::Element field);

    /**
     * @brief Frame a pop-up.
     * @param title Its title.
     * @param content What goes inside.
     * @param width Its width.
     * @return The element.
     */
    ftxui::Element frame(const std::string &title, ftxui::Element content, int width);

    /**
     * @brief What a routing strategy will do with the selected case, in words.
     * @param setup The setup with the routing settings.
     * @param kind The routing strategy.
     * @param c The case it reads values from.
     * @return The sentence.
     */
    std::string routingHint(const ScenarioSetup &setup, const std::string &kind, WorkflowInstance *c);

    /**
     * @brief What an assignment strategy will do, in words.
     * @param setup The setup with the rules.
     * @param kind The assignment strategy.
     * @return The sentence.
     */
    std::string assignmentHint(const ScenarioSetup &setup, const std::string &kind);
}

#endif
