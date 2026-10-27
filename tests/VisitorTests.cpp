#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "AuditExportVisitor.h"
#include "ProgressReportVisitor.h"
#include "Stage.h"
#include "Task.h"
#include "Staff.h"
#include "WorkItem.h"
#include "WorkflowEvent.h"

// ---------- helpers ----------

// Captures stdout. If f() throws, stdout capture is released first so one
// failing test doesn't break every test after it.
template <typename F>
std::string capture(F f)
{
    testing::internal::CaptureStdout();
    try
    {
        f();
    }
    catch (...)
    {
        testing::internal::GetCapturedStdout();
        throw;
    }
    return testing::internal::GetCapturedStdout();
}

// Staff is abstract, so tests need a concrete fake.
class FakeStaff : public Staff
{
public:
    FakeStaff(std::string n, std::string t, int r = 1)
        : name_(std::move(n)), title_(std::move(t)), rank_(r) {}
    std::string getName() override { return name_; }
    std::string getTitle() override { return title_; }
    int getRank() override { return rank_; }

private:
    std::string name_, title_;
    int rank_;
};

// A minimal concrete WorkItem. It does not override accept(), so it
// exercises WorkItem's default accept -> visitWorkItem behaviour.
class FakeItem : public WorkItem
{
public:
    FakeItem(std::string n, std::string t) : name_(std::move(n)), title_(std::move(t)) {}
    std::string getName() override { return name_; }
    std::string getTitle() override { return title_; }
    void printHierarchy(std::string indent) override
    {
        std::cout << indent << "Fake: " << name_ << "\n";
    }
    void handleRequest() override {}
    WorkItem *clone() override { return new FakeItem(name_, title_); }

private:
    std::string name_, title_;
};

// Records which visit method was called, so accept() dispatch can be tested
// without involving stdout.
class RecordingVisitor : public WorkflowVisitor
{
public:
    std::vector<std::string> calls;
    void visitStage(Stage *s) override { calls.push_back("stage:" + s->getName()); }
    void visitTask(Task *t) override { calls.push_back("task:" + t->getName()); }
    void visitWorkItem(WorkItem *w) override { calls.push_back("item:" + w->getName()); }
    void visitParticipant(Staff *p) override { calls.push_back("staff:" + p->getName()); }
    void visitHistoryEntry(WorkflowEvent *e) override { calls.push_back("event:" + e->getDescription()); }
};

// ---------- WorkflowEvent ----------

TEST(WorkflowEventTest, GettersReturnConstructorValues)
{
    WorkflowEvent e("STATE_CHANGE", "Moved to review", "2026-10-26 09:00");
    EXPECT_EQ(e.getType(), "STATE_CHANGE");
    EXPECT_EQ(e.getDescription(), "Moved to review");
    EXPECT_EQ(e.getTimestamp(), "2026-10-26 09:00");
}

// ---------- Accept dispatch ----------

TEST(AcceptTest, StageCallsVisitStage)
{
    Stage s("Design", "Design Phase");
    RecordingVisitor v;
    s.accept(&v);
    EXPECT_EQ(v.calls, std::vector<std::string>{"stage:Design"});
}

TEST(AcceptTest, TaskCallsVisitTask)
{ // fails until Task::accept is implemented
    Task t("Draft", "Draft spec");
    RecordingVisitor v;
    t.accept(&v);
    EXPECT_EQ(v.calls, std::vector<std::string>{"task:Draft"});
}

TEST(AcceptTest, PlainWorkItemCallsVisitWorkItem)
{
    FakeItem w("Misc", "Misc item");
    RecordingVisitor v;
    w.accept(&v);
    EXPECT_EQ(v.calls, std::vector<std::string>{"item:Misc"});
}

TEST(AcceptTest, NullVisitorIsSafe)
{
    Stage s("S", "");
    FakeItem w("W", "");
    EXPECT_NO_THROW(s.accept(nullptr));
    EXPECT_NO_THROW(w.accept(nullptr));
}

// ---------- ProgressReportVisitor ----------

TEST(ProgressReportVisitorTest, VisitStagePrintsNameOnly)
{
    ProgressReportVisitor v;
    Stage s("Design", "Design Phase");
    EXPECT_EQ(capture([&]
                      { v.visitStage(&s); }),
              "Stage: Design\n");
}

TEST(ProgressReportVisitorTest, VisitTaskPrintsNameOnly)
{
    ProgressReportVisitor v;
    Task t("Draft", "Draft spec");
    EXPECT_EQ(capture([&]
                      { v.visitTask(&t); }),
              "Task: Draft\n");
}

TEST(ProgressReportVisitorTest, VisitWorkItemPrintsNameAndHierarchy)
{ // fails until printHierarchy(0) fixed
    ProgressReportVisitor v;
    FakeItem w("Misc", "Misc item");
    EXPECT_EQ(capture([&]
                      { v.visitWorkItem(&w); }),
              "WorkItem: Misc\nFake: Misc\n");
}

TEST(ProgressReportVisitorTest, VisitParticipantPrintsName)
{ // fails until printHierarchy(0) fixed
    ProgressReportVisitor v;
    FakeStaff p("Alice", "Engineer");
    EXPECT_EQ(capture([&]
                      { v.visitParticipant(&p); }),
              "Participant: Alice\n");
}

TEST(ProgressReportVisitorTest, VisitHistoryEntryPrintsOnlyDescription)
{
    ProgressReportVisitor v;
    WorkflowEvent e("ACTION", "Task approved", "2026-10-26 09:00");
    EXPECT_EQ(capture([&]
                      { v.visitHistoryEntry(&e); }),
              "Task approved\n");
}

TEST(ProgressReportVisitorTest, ReportReflectsVisits)
{ // fails: report is never written
    ProgressReportVisitor v;
    Stage s("Design", "Design Phase");
    capture([&]
            { v.visitStage(&s); });
    EXPECT_NE(v.getReport().find("Design"), std::string::npos);
}

// ---------- AuditExportVisitor ----------

TEST(AuditExportVisitorTest, VisitStagePrintsNameTitleThenHierarchy)
{ // fails until fixed
    AuditExportVisitor v;
    Stage s("Design", "Design Phase");
    EXPECT_EQ(capture([&]
                      { v.visitStage(&s); }),
              "Stage: Design\nTitle: Design Phase\nStage: Design\n");
}

TEST(AuditExportVisitorTest, VisitTaskPrintsNameTitleThenHierarchy)
{ // fails until fixed
    AuditExportVisitor v;
    Task t("Draft", "Draft spec");
    EXPECT_EQ(capture([&]
                      { v.visitTask(&t); }),
              "Task: Draft\nTitle: Draft spec\nTask: Draft\n");
}

TEST(AuditExportVisitorTest, VisitWorkItemPrintsNameTitleThenHierarchy)
{ // fails until fixed
    AuditExportVisitor v;
    FakeItem w("Misc", "Misc item");
    EXPECT_EQ(capture([&]
                      { v.visitWorkItem(&w); }),
              "WorkItem: Misc\nTitle: Misc item\nFake: Misc\n");
}

TEST(AuditExportVisitorTest, VisitParticipantPrintsNameAndTitle)
{ // fails until fixed
    AuditExportVisitor v;
    FakeStaff p("Alice", "Engineer");
    EXPECT_EQ(capture([&]
                      { v.visitParticipant(&p); }),
              "Participant: Alice\nTitle: Engineer\n");
}

TEST(AuditExportVisitorTest, VisitHistoryEntryPrintsTimestampTypeDescription)
{
    AuditExportVisitor v;
    WorkflowEvent e("ACTION", "Task approved", "2026-10-26 09:00");
    EXPECT_EQ(capture([&]
                      { v.visitHistoryEntry(&e); }),
              "2026-10-26 09:00\nACTION\nTask approved\n");
}

TEST(AuditExportVisitorTest, AuditTrailHasHeaderAndFooter)
{ // fails: result is ignored
    AuditExportVisitor v;
    std::string trail = v.getAuditTrail();
    EXPECT_EQ(trail.rfind("Audit Trail\n", 0), 0u);
    EXPECT_NE(trail.find("End of Audit Trail"), std::string::npos);
}

// ---------- Stage ----------

TEST(StageTest, GettersReturnConstructorValues)
{
    Stage s("Design", "Design Phase");
    EXPECT_EQ(s.getName(), "Design");
    EXPECT_EQ(s.getTitle(), "Design Phase");
}

TEST(StageTest, PrintHierarchyIndentsNestedChildren)
{
    Stage root("Root", "");
    Stage* child = new Stage("Child", "");   // root owns it
    child->addMember(new Task("T1", ""));    // child owns that
    root.addMember(child);
    EXPECT_EQ(capture([&]
                      { root.printHierarchy(""); }),
              "Stage: Root\n  Stage: Child\n    Task: T1\n");
}

TEST(StageTest, AddNullMemberIsIgnored)
{
    Stage s("S", "");
    s.addMember(nullptr);
    EXPECT_EQ(capture([&]
                      { s.printHierarchy(""); }),
              "Stage: S\n");
}

TEST(StageTest, RemoveMemberRemovesOnlyThatMember)
{
    Stage s("S", "");
    Task* a = new Task("A", "");
    Task* b = new Task("B", "");   // s owns b
    s.addMember(a);
    s.addMember(b);
    s.removeMember(a);             // detached, so this test owns a again
    EXPECT_EQ(capture([&]
                      { s.printHierarchy(""); }),
              "Stage: S\n  Task: B\n");
    delete a;
}

TEST(StageTest, RemoveUnknownOrNullMemberIsSafe)
{
    Stage s("S", "");
    Task stranger("X", "");
    EXPECT_NO_THROW(s.removeMember(&stranger));
    EXPECT_NO_THROW(s.removeMember(nullptr));
}

TEST(StageTest, CloneCopiesNameTitleAndIsNewObject)
{
    Stage s("S", "Title");
    WorkItem *copy = s.clone();
    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy, &s);
    EXPECT_EQ(copy->getName(), "S");
    EXPECT_EQ(copy->getTitle(), "Title");
    delete copy;
}

TEST(StageTest, CloneDeepCopiesTasks)
{
    Stage s("S", "Title");
    s.addMember(new Task("T1", ""));
    WorkItem *copy = s.clone();
    ASSERT_NE(copy, nullptr);
    EXPECT_EQ(capture([&]
                      { copy->printHierarchy(""); }),
              capture([&]
                      { s.printHierarchy(""); }));
    delete copy; // the clone owns its copied children and free them
}

// ---------- Task ----------

TEST(TaskTest, GettersReturnConstructorValues)
{
    Task t("Draft", "Draft spec");
    EXPECT_EQ(t.getName(), "Draft");
    EXPECT_EQ(t.getTitle(), "Draft spec");
}

TEST(TaskTest, PrintHierarchyUsesIndent)
{
    Task t("Draft", "");
    EXPECT_EQ(capture([&]
                      { t.printHierarchy("    "); }),
              "    Task: Draft\n");
}

TEST(TaskTest, CloneReturnsIndependentCopy)
{ // fails: returns nullptr
    Task t("Draft", "Title");
    WorkItem *copy = t.clone();
    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy, &t);
    EXPECT_EQ(copy->getName(), "Draft");
    EXPECT_EQ(copy->getTitle(), "Title");
    delete copy;
}