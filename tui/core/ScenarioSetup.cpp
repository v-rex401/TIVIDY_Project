/**
 * @file ScenarioSetup.cpp
 * @brief Checks that a ScenarioSetup can be built.
 * @author JP
 * @date 2026-10-31
 */

#include "ScenarioSetup.h"

#include <algorithm>
#include <set>

namespace
{
    /**
     * @brief Whether a value is one of the allowed names.
     * @param value The value to look for.
     * @param allowed The allowed names.
     * @return True if found.
     */
    bool oneOf(const std::string &value, const std::vector<std::string> &allowed)
    {
        return std::find(allowed.begin(), allowed.end(), value) != allowed.end();
    }
}

bool ScenarioSetup::hasStage(const std::string &stage) const
{
    for (const StageSpec &s : stages)
    {
        if (s.name == stage)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> ScenarioSetup::problems() const
{
    std::vector<std::string> found;

    if (workflowName.empty())
    {
        found.push_back("Give the workflow a name.");
    }
    if (stages.empty())
    {
        found.push_back("Add at least one stage.");
    }
    if (people.empty())
    {
        found.push_back("Add at least one person, or nobody can be given work.");
    }

    std::set<std::string> staffNames;
    std::set<std::string> teamNames;
    for (const TeamSpec &team : teams)
    {
        if (!staffNames.insert(team.name).second)
        {
            found.push_back("Two people or teams are called \"" + team.name + "\".");
        }
        if (!team.parent.empty() && teamNames.count(team.parent) == 0)
        {
            found.push_back("Team \"" + team.name + "\" sits in \"" + team.parent + "\", which has to be added first.");
        }
        teamNames.insert(team.name);
    }
    for (const PersonSpec &person : people)
    {
        if (!staffNames.insert(person.name).second)
        {
            found.push_back("Two people or teams are called \"" + person.name + "\".");
        }
        if (!person.team.empty() && teamNames.count(person.team) == 0)
        {
            found.push_back(person.name + " is in team \"" + person.team + "\", which doesn't exist.");
        }
    }
    for (const std::string &name : chain)
    {
        if (staffNames.count(name) == 0)
        {
            found.push_back("The escalation chain names \"" + name + "\", who isn't set up.");
        }
    }

    std::set<std::string> stageNames;
    for (const StageSpec &stage : stages)
    {
        if (!stageNames.insert(stage.name).second)
        {
            found.push_back("Two stages are called \"" + stage.name + "\".");
        }
    }

    if (!oneOf(builder, {"Sequential", "Escalating"}))
    {
        found.push_back("Unknown builder \"" + builder + "\".");
    }
    if (!oneOf(assignment, {"RoleBased", "WorkloadBased", "RuleBased"}))
    {
        found.push_back("Unknown assignment \"" + assignment + "\".");
    }
    if (!oneOf(routing.kind, {"SeverityBased", "ApprovalChain", "EventBased"}))
    {
        found.push_back("Unknown routing \"" + routing.kind + "\".");
    }

    // Every stage a routing setting points at must exist, whichever routing is the default.
    std::vector<std::string> pointedAt = {routing.normalStage, routing.emergencyStage,
                                          routing.standardStage, routing.escalationStage};
    for (const ThresholdSpec &threshold : routing.thresholds)
    {
        pointedAt.push_back(threshold.stage);
    }
    for (const std::string &stage : pointedAt)
    {
        if (!stage.empty() && !hasStage(stage))
        {
            found.push_back("Routing sends work to \"" + stage + "\", but there is no such stage.");
        }
    }

    if (routing.kind == "SeverityBased" && (routing.normalStage.empty() || routing.emergencyStage.empty()))
    {
        found.push_back("Severity routing needs a normal and an emergency stage.");
    }
    if (routing.kind == "ApprovalChain" &&
        (routing.metricKey.empty() || routing.standardStage.empty() || routing.escalationStage.empty()))
    {
        found.push_back("Approval-chain routing needs a value to check and two stages.");
    }
    if (routing.kind == "EventBased" && (routing.metricKey.empty() || routing.thresholds.empty()))
    {
        found.push_back("Event routing needs a value to check and at least one threshold.");
    }

    return found;
}
