/**
 * @file TextDemo.cpp
 * @brief Implementation of the scripted text run.
 * @author JP
 * @date 2026-10-31
 */

#include "TextDemo.h"

#include <string>

#include "EventLog.h"
#include "Session.h"
#include "State.h"
#include "WorkflowInstance.h"

namespace
{
    /**
     * @brief Print a heading.
     * @param out Where to print.
     * @param text The heading.
     */
    void heading(std::ostream &out, const std::string &text)
    {
        out << "\n" << text << "\n" << std::string(text.size(), '-') << "\n";
    }

    /**
     * @brief Print the timeline entries added since the last call.
     * @param out Where to print.
     * @param session The session.
     * @param printed How many entries were already printed; updated.
     */
    void printNew(std::ostream &out, const Session &session, std::size_t &printed)
    {
        const std::vector<TimelineEntry> &all = session.timeline();
        for (; printed < all.size(); ++printed)
        {
            const TimelineEntry &e = all[printed];
            out << "  " << (e.caseId.empty() ? "     " : e.caseId) << "  " << (e.refused ? "refused: " : "")
                << e.text << "   [" << e.pattern << "]\n";
        }
    }
}

int TextDemo::run(const ScenarioSetup &setup, std::ostream &out)
{
    Session session(setup);
    std::size_t printed = 0;

    out << "TIVIDY: " << setup.name << "\n" << setup.summary << "\n";
    out << "(No terminal detected, so this is the scripted run. Start tividy in a terminal for the TUI.)\n";

    heading(out, "Set up (Builder, Composite, Chain of Responsibility)");
    printNew(out, session, printed);
    for (const std::string &line : session.staffTree())
    {
        out << "  " << line << "\n";
    }
    out << "  Escalation chain:";
    for (const ChainStep &step : session.chain())
    {
        out << " " << step.name << " (" << step.rank << ")";
    }
    out << "\n";

    heading(out, "Two cases from one definition (Facade, Prototype, State)");
    WorkflowInstance *first = session.openCase();
    WorkflowInstance *second = session.openCase();
    printNew(out, session, printed);

    heading(out, "Adding work (Strategy, Adapter)");
    for (const WorkSpec &work : setup.sampleWork)
    {
        session.addWork(first, work, setup.assignment, setup.routing.kind);
    }
    if (!session.dataKeys().empty())
    {
        std::string key = session.dataKeys().front();
        session.setData(first, key, first->getData(key) * 10);
        session.addWork(first, setup.sampleWork.front(), setup.assignment, setup.routing.kind);
    }
    printNew(out, session, printed);

    heading(out, "Commands with undo (Command, Memento, State)");
    session.reject(first);
    session.approve(first);
    session.approve(first);
    session.approve(first);
    session.undo(first);
    session.cancel(second);
    printNew(out, session, printed);
    out << "  " << first->getId() << " is " << first->getState()->getName() << ", " << second->getId() << " is "
        << second->getState()->getName() << "\n";

    heading(out, "Escalation (Chain of Responsibility)");
    session.escalate(first, 3);
    session.escalate(first, 99);
    printNew(out, session, printed);

    heading(out, "Reports (Visitor, Observer)");
    out << session.progressReport(first);
    printNew(out, session, printed);
    out << "  Observers saw " << first->getEventLog()->getEntries().size() << " state changes on " << first->getId()
        << "; the notification service sent " << session.notificationsSent() << " notifications.\n";

    return 0;
}
