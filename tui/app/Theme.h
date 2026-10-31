/**
 * @file Theme.h
 * @brief Colours and small building blocks shared by every TUI screen.
 * @author JP
 * @date 2026-10-31
 */

#ifndef THEME_H
#define THEME_H

#include <functional>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

/**
 * @brief The look of the TUI in one place.
 *
 * One accent colour marks "the thing you just did"; states have their own
 * colour so a case's status reads at a glance; everything else stays in the
 * terminal's own colours.
 */
namespace Theme
{
    /** @return Colour for whatever just happened. */
    ftxui::Color accent();

    /** @return Colour for something the engine refused. */
    ftxui::Color refused();

    /**
     * @brief Colour of a workflow state.
     * @param state "Pending", "Active", "Completed" or "Cancelled".
     * @return Its colour.
     */
    ftxui::Color stateColour(const std::string &state);

    /**
     * @brief A state name in its colour.
     * @param state The state.
     * @return The element.
     */
    ftxui::Element stateLabel(const std::string &state);

    /**
     * @brief A key and what it does, e.g. "a approve".
     * @param key The key.
     * @param label What it does.
     * @return The element.
     */
    ftxui::Element keyHint(const std::string &key, const std::string &label);

    /**
     * @brief A section title.
     * @param text The title.
     * @return The element.
     */
    ftxui::Element heading(const std::string &text);

    /**
     * @brief A box with a title, used for every panel.
     * @param title The title.
     * @param content What goes inside.
     * @return The element.
     */
    ftxui::Element panel(const std::string &title, ftxui::Element content);

    /**
     * @brief Flat button style: plain text, inverted when focused.
     * @return The option.
     */
    ftxui::ButtonOption button();

    /**
     * @brief Single-line input that underlines itself when focused.
     * @return The option.
     */
    ftxui::InputOption input();

    /**
     * @brief Pick one of a few options on one line; the chosen one is in the accent colour.
     * @param entries The options (read every time it is drawn, so the list may change).
     * @param selected Index of the chosen option.
     * @param onChange Called after the choice changes.
     * @return The component.
     */
    ftxui::Component choice(const std::vector<std::string> *entries, int *selected,
                            std::function<void()> onChange = {});
}

#endif
