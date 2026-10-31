/**
 * @file Theme.cpp
 * @brief Implementation of the TUI's look.
 * @author JP
 * @date 2026-10-31
 */

#include "Theme.h"

#include <map>

using namespace ftxui;

Color Theme::accent()
{
    return Color::RGB(250, 179, 95);
}

Color Theme::refused()
{
    return Color::RGB(224, 108, 117);
}

Color Theme::stateColour(const std::string &state)
{
    static const std::map<std::string, Color> colours = {
        {"Pending", Color::RGB(200, 200, 200)},
        {"Active", Color::RGB(126, 203, 140)},
        {"Completed", Color::RGB(140, 170, 250)},
        {"Cancelled", Color::RGB(224, 108, 117)},
    };
    std::map<std::string, Color>::const_iterator found = colours.find(state);
    return found == colours.end() ? Color::Default : found->second;
}

Element Theme::stateLabel(const std::string &state)
{
    return text(state) | bold | color(stateColour(state));
}

Element Theme::keyHint(const std::string &key, const std::string &label)
{
    return hbox({text(key) | bold, text(" " + label + "   ") | dim});
}

Element Theme::heading(const std::string &title)
{
    return text(title) | bold;
}

Element Theme::panel(const std::string &title, Element content)
{
    return window(text(" " + title + " ") | bold, content | flex, LIGHT);
}

ButtonOption Theme::button()
{
    ButtonOption option;
    option.transform = [](const EntryState &state)
    {
        Element label = text(" " + state.label + " ");
        if (state.focused)
        {
            label = label | inverted;
        }
        return label;
    };
    return option;
}

InputOption Theme::input()
{
    InputOption option;
    option.multiline = false;
    option.transform = [](InputState state)
    {
        if (state.is_placeholder)
        {
            state.element |= dim;
        }
        if (state.focused)
        {
            state.element |= underlined;
        }
        return state.element;
    };
    return option;
}
