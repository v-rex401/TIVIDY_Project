/**
 * @file MainUi.h
 * @brief Which pop-ups are open on the main screen, and the actions its buttons and keys share.
 * @author JP
 * @date 2026-10-31
 */

#ifndef MAINUI_H
#define MAINUI_H

#include <functional>
#include <string>

#include "Session.h"

/**
 * @brief Pop-up flags and pop-up results for the main screen.
 *
 * Only one pop-up is open at a time. While one is open the main screen's
 * single-key shortcuts are off, so typing into a form never triggers an action.
 */
struct MainUi
{
    bool addWork = false;   ///< "Add work" form.
    bool escalate = false;  ///< "Escalate" form.
    bool value = false;     ///< "Change a value" form.
    bool report = false;    ///< Visitor reports.
    bool help = false;      ///< Key and pattern help.
    bool addPerson = false; ///< "Add a person" form.

    std::string lastWork;          ///< Name of the work added last, highlighted on the board.
    EscalationResult lastLadder;   ///< Result of the last escalation, drawn as a ladder.
    bool hasLadder = false;        ///< Whether lastLadder holds anything yet.

    /** @return True if any pop-up is open. */
    bool anyOpen() const
    {
        return addWork || escalate || value || report || help || addPerson;
    }

    /** @brief Close every pop-up. */
    void closeAll()
    {
        addWork = escalate = value = report = help = addPerson = false;
    }
};

/** @brief What the case screen's buttons do; the same functions are bound to keys. */
struct CaseActions
{
    std::function<void()> approve;   ///< a
    std::function<void()> reject;    ///< r
    std::function<void()> retry;     ///< t
    std::function<void()> cancel;    ///< c
    std::function<void()> undo;      ///< u
    std::function<void()> execute;   ///< x
    std::function<void()> addWork;   ///< w
    std::function<void()> escalate;  ///< e
    std::function<void()> editValue; ///< v
    std::function<void()> reports;   ///< p
    std::function<void()> newCase;   ///< n
};

#endif
