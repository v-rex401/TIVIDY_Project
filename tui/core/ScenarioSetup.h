/**
 * @file ScenarioSetup.h
 * @brief Plain data describing everything set up before a workflow runs.
 * @author JP
 * @date 2026-10-31
 */

#ifndef SCENARIOSETUP_H
#define SCENARIOSETUP_H

#include <string>
#include <utility>
#include <vector>

/** @brief A team in the staff hierarchy (Composite). */
struct TeamSpec
{
    std::string name;   ///< e.g. "Ward".
    std::string title;  ///< Role of the team as a whole.
    int rank = 1;       ///< Authority level used by the escalation chain.
    std::string parent; ///< Name of the team it sits in, or empty for top level.
};

/** @brief One employee. */
struct PersonSpec
{
    std::string name;  ///< e.g. "Jane Mokoena".
    std::string title; ///< Job title; role-based assignment matches on it.
    int rank = 1;      ///< Authority level used by the escalation chain.
    std::string team;  ///< Name of the team they belong to, or empty for none.
};

/** @brief One stage of the workflow and the tasks it starts with. */
struct StageSpec
{
    std::string name;               ///< e.g. "Admission".
    std::vector<std::string> tasks; ///< Task titles, in order.
};

/** @brief "Values at or above limit go to stage" for event-based routing. */
struct ThresholdSpec
{
    double limit = 0; ///< Lowest value that triggers this threshold.
    std::string stage; ///< Stage the work goes to.
};

/** @brief "If field equals value, give it to someone with this title" for rule-based assignment. */
struct RuleSpec
{
    std::string field;       ///< "name", "title" or "requiredPosition".
    std::string value;       ///< Value the field must equal.
    std::string targetTitle; ///< Title of the person who gets the work.
};

/** @brief A ready-made piece of work, so the demo does not need typing. */
struct WorkSpec
{
    std::string name;     ///< What has to be done.
    std::string role;     ///< Job title needed (role-based assignment).
    std::string severity; ///< "Low", "Medium", "High" or "Critical" (severity-based routing).
};

/**
 * @brief Settings for all three routing strategies, so any of them can be picked at run time.
 */
struct RoutingSpec
{
    std::string kind = "EventBased"; ///< Default: "SeverityBased", "ApprovalChain" or "EventBased".

    std::string normalStage;    ///< SeverityBased: where Low/Medium work goes.
    std::string emergencyStage; ///< SeverityBased: where High/Critical work goes.

    std::string metricKey;       ///< ApprovalChain and EventBased: the case value they read.
    double approvalLimit = 0;    ///< ApprovalChain: values above this escalate.
    std::string standardStage;   ///< ApprovalChain: where work goes up to the limit.
    std::string escalationStage; ///< ApprovalChain: where work goes above the limit.

    std::vector<ThresholdSpec> thresholds; ///< EventBased: checked in order, first match wins.
};

/**
 * @brief Steps 1 to 8 of setting up a workflow, as data.
 *
 * A ready-made scenario fills this in code; the "build your own" screens
 * fill it from what the user types. Session turns it into engine objects.
 * The engine never sees this struct, which keeps src/ free of scenario detail.
 */
struct ScenarioSetup
{
    std::string name;    ///< Shown in the title bar, e.g. "Hospital admissions".
    std::string summary; ///< One line for the scenario picker.

    std::vector<TeamSpec> teams;    ///< Teams, parents before children.
    std::vector<PersonSpec> people; ///< Employees.
    std::vector<std::string> chain; ///< Names of staff in escalation order, most junior first.

    std::string workflowName;            ///< Name of the workflow definition.
    std::string builder = "Sequential";  ///< "Sequential" or "Escalating".
    std::vector<StageSpec> stages;       ///< Stages in order.

    std::string assignment = "RoleBased"; ///< "RoleBased", "WorkloadBased" or "RuleBased".
    std::vector<RuleSpec> rules;          ///< Used by RuleBased.
    RoutingSpec routing;                  ///< Routing settings.

    std::vector<std::pair<std::string, double>> startingData; ///< Values every new case starts with.
    std::vector<WorkSpec> sampleWork;                         ///< Quick picks in the "Add work" form.

    /**
     * @brief Find the problems that would stop this setup from working.
     *
     * Checks that names line up: teams, chain members, and the stages the
     * routing settings point at must all exist.
     *
     * @return One plain sentence per problem; empty when the setup is usable.
     */
    std::vector<std::string> problems() const;

    /**
     * @brief Whether a stage with this name exists.
     * @param stage The stage name.
     * @return True if one of the stages has that name.
     */
    bool hasStage(const std::string &stage) const;
};

#endif
