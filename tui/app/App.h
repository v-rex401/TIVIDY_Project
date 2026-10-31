/**
 * @file App.h
 * @brief Starts the full-screen TUI.
 * @author JP
 * @date 2026-10-31
 */

#ifndef APP_H
#define APP_H

/**
 * @brief The TUI (the client of the WorkflowFacade).
 */
namespace App
{
    /**
     * @brief Show the start screen and run until the user quits.
     * @return The program's exit code.
     */
    int run();
}

#endif
