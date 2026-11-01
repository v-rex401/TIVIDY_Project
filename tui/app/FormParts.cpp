/**
 * @file FormParts.cpp
 * @brief Implementation of the pieces shared by the pop-up forms.
 * @author JP
 * @date 2026-10-31
 */

#include "FormParts.h"

#include <algorithm>
#include <sstream>

#include "Theme.h"
#include "WorkflowInstance.h"

using namespace ftxui;

namespace FormParts
{
    const std::vector<std::string> kAssignmentKinds = {"RoleBased", "WorkloadBased", "RuleBased"};
    const std::vector<std::string> kRoutingKinds = {"SeverityBased", "ApprovalChain", "EventBased"};

    std::string number(double value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }

    int toInt(const std::string &text, int fallback)
    {
        try
        {
            return std::stoi(text);
        }
        catch (...)
        {
            return fallback;
        }
    }

    int indexOf(const std::vector<std::string> &list, const std::string &name)
    {
        std::vector<std::string>::const_iterator found = std::find(list.begin(), list.end(), name);
        return found == list.end() ? 0 : static_cast<int>(found - list.begin());
    }

    Element row(const std::string &label, Element field)
    {
        return hbox({text(label) | size(WIDTH, EQUAL, 15) | dim, field | flex});
    }

    Element frame(const std::string &title, Element content, int width)
    {
        return window(text(" " + title + " ") | bold | color(Theme::accent()), content) |
               size(WIDTH, EQUAL, width) | clear_under;
    }

    std::string routingHint(const ScenarioSetup &setup, const std::string &kind, WorkflowInstance *c)
    {
        const RoutingSpec &r = setup.routing;
        std::string value = c != nullptr && !r.metricKey.empty() ? number(c->getData(r.metricKey)) : "?";
        if (kind == "SeverityBased")
        {
            return "High or Critical → " + r.emergencyStage + ", anything else → " + r.normalStage;
        }
        if (kind == "ApprovalChain")
        {
            return r.metricKey + " is " + value + ": above " + number(r.approvalLimit) + " → " + r.escalationStage +
                   ", otherwise → " + r.standardStage;
        }
        std::string text = r.metricKey + " is " + value + ":";
        for (const ThresholdSpec &t : r.thresholds)
        {
            text += " " + number(t.limit) + "+ → " + t.stage + ",";
        }
        if (!text.empty() && text.back() == ',')
        {
            text.pop_back();
        }
        return text;
    }

    std::string assignmentHint(const ScenarioSetup &setup, const std::string &kind)
    {
        if (kind == "RoleBased")
        {
            return "the first person whose title is exactly what it needs";
        }
        if (kind == "WorkloadBased")
        {
            return "whoever has the least work across running cases (ignores the title)";
        }
        std::string text = "the first rule that matches:";
        for (std::size_t i = 0; i < setup.rules.size() && i < 3; ++i)
        {
            text += " " + setup.rules[i].field + "=" + setup.rules[i].value + " → " + setup.rules[i].targetTitle + ";";
        }
        return text;
    }
}
