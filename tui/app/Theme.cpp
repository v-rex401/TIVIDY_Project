/**
 * @file Theme.cpp
 * @brief Implementation of the TUI's look.
 * @author JP
 * @date 2026-10-31
 */

#include "Theme.h"

#include <utility>

#include <map>

#include <ftxui/component/event.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

namespace
{
    /** @brief Draws its child, then gives every cell the same text and background colour. */
    class SelectedNode : public Node
    {
    public:
        /**
         * @brief Wrap an element.
         * @param child The element to highlight.
         * @param background The highlight colour.
         */
        SelectedNode(Element child, Color background) : Node({std::move(child)}), background(background) {}

        void ComputeRequirement() override
        {
            Node::ComputeRequirement();
            requirement_ = children_[0]->requirement();
        }

        void SetBox(Box box) override
        {
            Node::SetBox(box);
            children_[0]->SetBox(box);
        }

        void Render(Screen &screen) override
        {
            Node::Render(screen);
            for (int y = box_.y_min; y <= box_.y_max; ++y)
            {
                for (int x = box_.x_min; x <= box_.x_max; ++x)
                {
                    Pixel &pixel = screen.PixelAt(x, y);
                    pixel.background_color = background;
                    pixel.foreground_color = Color::RGB(20, 20, 20);
                    pixel.dim = false;
                    pixel.inverted = false;
                }
            }
        }

    private:
        Color background; ///< The highlight colour.
    };

    /** @brief Hands every key to its child except Tab, which goes up to the form so it moves to the next field. */
    class TabMovesOn : public ComponentBase
    {
    public:
        /**
         * @brief Wrap a component.
         * @param child The component that would otherwise keep Tab.
         */
        explicit TabMovesOn(Component child)
        {
            Add(std::move(child));
        }

        bool OnEvent(Event event) override
        {
            if (event == Event::Tab || event == Event::TabReverse)
            {
                return false;
            }
            return ComponentBase::OnEvent(event);
        }
    };
}

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

Element Theme::selected(Element element, Color background)
{
    return std::make_shared<SelectedNode>(std::move(element), background);
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

Element Theme::panel(const std::string &title, ftxui::Element content)
{
    return window(text(" " + title + " ") | bold, content, LIGHT);
}

ButtonOption Theme::button()
{
    ButtonOption option;
    option.transform = [](const EntryState &state)
    {
        Element label = text(" " + state.label + " ");
        if (state.focused)
        {
            label = selected(label);
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

Component Theme::choice(const std::vector<std::string> *entries, int *selected, std::function<void()> onChange)
{
    MenuOption option = MenuOption::Horizontal();
    option.entries_option.transform = [](const EntryState &state)
    {
        Element label = text(" " + state.label + " ");
        label = state.active ? label | bold | color(accent()) : label | dim;
        return state.focused ? Theme::selected(label) : label;
    };
    option.elements_infix = [] { return text(" "); };
    if (onChange)
    {
        option.on_change = std::move(onChange);
    }
    return tabMovesOn(Menu(entries, selected, option));
}

Component Theme::tabMovesOn(Component component)
{
    return std::make_shared<TabMovesOn>(std::move(component));
}
