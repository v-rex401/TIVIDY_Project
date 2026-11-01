/**
 * @file TextDemo.h
 * @brief A scripted, plain-text run of a scenario for when there is no terminal.
 * @author JP
 * @date 2026-10-31
 */

#ifndef TEXTDEMO_H
#define TEXTDEMO_H

#include <ostream>

#include "ScenarioSetup.h"

/**
 * @brief Runs the same actions the TUI offers, one after another, and prints what happened.
 *
 * Used when stdin is not a terminal (CI, Valgrind, piping), so every pattern
 * is still exercised through the Session and the program exits by itself.
 */
namespace TextDemo
{
    /**
     * @brief Run the script on one scenario.
     * @param setup The scenario to run.
     * @param out Where to print the story.
     * @return 0, the program's exit code.
     */
    int run(const ScenarioSetup &setup, std::ostream &out);
}

#endif
