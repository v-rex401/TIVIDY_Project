/**
 * @file Scenarios.h
 * @brief The ready-made scenarios used in the demo.
 * @author JP
 * @date 2026-10-31
 */

#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <vector>

#include "ScenarioSetup.h"

/**
 * @brief Ready-made setups, so the demo can skip straight to running workflows.
 *
 * All industry detail lives here, as strings, and reaches the engine through
 * the Director and the strategies. Nothing in src/ knows about hospitals or claims.
 */
namespace Scenarios
{
    /**
     * @brief Hospital patient admissions (sequential stages, event-based routing on a triage score).
     * @return The setup.
     */
    ScenarioSetup hospital();

    /**
     * @brief Insurance claims (escalating builder, approval-chain routing on the claim amount).
     * @return The setup.
     */
    ScenarioSetup insurance();

    /**
     * @brief The S.H.I.E.L.D. threat response from the original main.cpp (severity routing, team chain).
     * @return The setup.
     */
    ScenarioSetup shield();

    /**
     * @brief Every ready-made scenario, in the order the picker shows them.
     * @return The setups.
     */
    std::vector<ScenarioSetup> all();
}

#endif
