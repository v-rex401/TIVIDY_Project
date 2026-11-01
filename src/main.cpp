/**
 * @file main.cpp
 * @brief Starts TIVIDY: the full-screen TUI in a terminal, a scripted text run otherwise.
 * @author JP
 * @date 2026-10-31
 */

#include <iostream>

#include <unistd.h>

#include "App.h"
#include "Scenarios.h"
#include "TextDemo.h"

/**
 * @brief Run the TUI when both ends are a terminal; otherwise run the scripted demo and exit.
 *
 * CI and Valgrind start the program with stdin from /dev/null, where a
 * full-screen UI would wait forever. The scripted run goes through the same
 * Session, so every pattern is still exercised there.
 *
 * @return The exit code.
 */
int main()
{
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
    {
        return TextDemo::run(Scenarios::shield(), std::cout);
    }
    return App::run();
}
