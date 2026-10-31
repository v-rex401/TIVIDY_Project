/**
 * @file WorkflowView.cpp
 * @brief Implementation of the Workflow tab.
 * @author JP
 * @date 2026-10-31
 */

#include "WorkflowView.h"

#include <string>
#include <vector>

#include "FormParts.h"
#include "Stage.h"
#include "Theme.h"
#include "WorkflowDefinition.h"

using namespace ftxui;
using namespace FormParts;

namespace
{
    /**
     * @brief A "label  value" line.
     * @param label The label.
     * @param value The value.
     * @return The element.
     */
    Element fact(const std::string &label, const std::string &value)
    {
        return hbox({text(label) | dim | size(WIDTH, EQUAL, 16), text(value) | bold});
    }
}

Component WorkflowView(AppState &app)
{
    return Renderer([&app]
    {
        if (!app.session)
        {
            return text("");
        }
        Session &session = *app.session;
        const ScenarioSetup &setup = session.setup();
        WorkflowDefinition *definition = session.definition();

        // Read the handler links back from the built definition, so this shows what the Builder really did.
        std::string links;
        std::vector<WorkItem *> *stages = definition->getStages();
        WorkItem *start = nullptr;
        for (WorkItem *candidate : *stages)
        {
            bool pointedAt = false;
            for (WorkItem *other : *stages)
            {
                pointedAt = pointedAt || other->getNextHandler() == candidate;
            }
            if (!pointedAt)
            {
                start = candidate;
                break;
            }
        }
        for (WorkItem *s = start; s != nullptr; s = s->getNextHandler())
        {
            links += (links.empty() ? "" : " → ") + s->getName();
        }

        std::string builderMeaning = setup.builder == "Escalating"
                                         ? "links each stage back to the one before it, so a request climbs back up"
                                         : "links each stage to the next one, in the order they were given";

        Elements definitionRows = {
            fact("Name", definition->getName()),
            fact("Made by", setup.builder + " builder, directed by WorkflowDirector"),
            hbox({text("") | size(WIDTH, EQUAL, 16), paragraph("The " + setup.builder + " builder " + builderMeaning + ".") | dim}),
            fact("Stage links", links),
            fact("Cases so far", std::to_string(session.cases().size()) + ", each with its own copy of these stages"),
        };

        Elements stageRows;
        for (WorkItem *item : *stages)
        {
            Stage *stage = dynamic_cast<Stage *>(item);
            if (stage == nullptr)
            {
                continue;
            }
            stageRows.push_back(text(stage->getName()) | bold);
            for (WorkItem *task : stage->getMembers())
            {
                stageRows.push_back(text("   " + task->getName()));
            }
        }

        const RoutingSpec &r = setup.routing;
        Elements strategyRows = {
            text("Who gets new work") | bold,
            fact("Default", setup.assignment),
            hbox({text("") | size(WIDTH, EQUAL, 16), paragraph(assignmentHint(setup, setup.assignment)) | dim}),
            text(""),
            text("Where new work goes") | bold,
            fact("Default", r.kind),
            fact("Severity", "High/Critical → " + r.emergencyStage + ", else → " + r.normalStage),
            fact("Approval chain", r.metricKey + " above " + number(r.approvalLimit) + " → " + r.escalationStage +
                                       ", else → " + r.standardStage),
        };
        std::string thresholds;
        for (const ThresholdSpec &t : r.thresholds)
        {
            thresholds += (thresholds.empty() ? "" : ", ") + number(t.limit) + "+ → " + t.stage;
        }
        strategyRows.push_back(fact("Event", r.metricKey + ": " + thresholds));
        strategyRows.push_back(text(""));
        strategyRows.push_back(text("Values every case starts with") | bold);
        for (const std::pair<std::string, double> &value : setup.startingData)
        {
            strategyRows.push_back(fact(value.first, number(value.second)));
        }
        strategyRows.push_back(text(""));
        strategyRows.push_back(paragraph("Any of the three can be picked when adding work, so the same task can be "
                                         "sent in different ways without changing the workflow (Strategy).") | dim);

        return vbox({
            Theme::panel("The workflow definition (Builder product)", vbox(definitionRows)),
            hbox({
                Theme::panel("Stages and their starting tasks", vbox(stageRows) | yframe) | size(WIDTH, EQUAL, 42),
                Theme::panel("Strategies", vbox(strategyRows) | yframe) | flex,
            }) | flex,
        });
    });
}
