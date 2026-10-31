/**
 * @file ValueForm.cpp
 * @brief The "Change a value" pop-up.
 * @author JP
 * @date 2026-10-31
 */

#include "Forms.h"

#include <string>
#include <vector>

#include <ftxui/component/event.hpp>

#include "WorkflowInstance.h"
#include "FormParts.h"
#include "Theme.h"

using namespace ftxui;
using namespace FormParts;

Popup ValueForm(AppState &app, std::shared_ptr<MainUi> ui)
{
    /** @brief The form's fields. */
    struct Fields
    {
        int key = 0;
        std::string value;
        std::string newKey;
        std::vector<std::string> keys;
    };
    std::shared_ptr<Fields> f = std::make_shared<Fields>();

    auto set = [&app, ui, f]
    {
        std::string key = !f->newKey.empty() ? f->newKey : (f->keys.empty() ? "" : f->keys[f->key]);
        double value = 0;
        try
        {
            value = std::stod(f->value);
        }
        catch (...)
        {
            app.say("\"" + f->value + "\" isn't a number.", false);
            return;
        }
        ActionResult result = app.session->setData(app.currentCase(), key, value);
        app.say(result.message, result.changed);
        if (result.changed)
        {
            ui->value = false;
        }
    };

    Component key = Toggle(&f->keys, &f->key);
    InputOption option = Theme::input();
    option.on_enter = set;
    Component value = Input(&f->value, "a number", option);
    Component newKey = Input(&f->newKey, "or a new value name", option);
    Component ok = Button("Set", set, Theme::button());
    Component cancel = Button("Cancel", [ui] { ui->value = false; }, Theme::button());
    Component form = Container::Vertical({key, value, newKey, Container::Horizontal({ok, cancel})});

    Component view = Renderer(form, [&app, f, key, value, newKey, ok, cancel]
    {
        WorkflowInstance *c = app.currentCase();
        std::string now = (c != nullptr && !f->keys.empty()) ? number(c->getData(f->keys[f->key])) : "-";
        Element content = vbox({
            paragraph("Routing reads these values, so changing one changes where the next work goes. "
                      "Only this case changes.") | dim,
            text(""),
            row("Value", key->Render()),
            row("Now", text(now) | bold),
            row("New value", value->Render()),
            row("", newKey->Render()),
            text(""),
            hbox({ok->Render(), text("  "), cancel->Render(), filler(), text("esc closes") | dim}),
        });
        return frame("Change a value on " + (c != nullptr ? c->getId() : std::string("?")), content, 64);
    });

    auto onOpen = [&app, f]
    {
        f->keys = app.session->dataKeys();
        f->key = 0;
        f->value.clear();
        f->newKey.clear();
    };
    return {view, onOpen};
}
