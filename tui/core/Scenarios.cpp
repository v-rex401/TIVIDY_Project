/**
 * @file Scenarios.cpp
 * @brief Data for the ready-made scenarios, taken from the team's scenario write-ups.
 * @author JP
 * @date 2026-10-31
 */

#include "Scenarios.h"

ScenarioSetup Scenarios::hospital()
{
    ScenarioSetup s;
    s.name = "Hospital admissions";
    s.summary = "A patient moves from admission to discharge. The triage score decides where new work goes.";

    s.teams = {
        {"Front Desk", "Admissions", 1, ""},
        {"Ward", "Nursing", 2, ""},
        {"Doctors", "Medical", 3, ""},
    };
    s.people = {
        {"Ann Botha", "Admission Clerk", 1, "Front Desk"},
        {"Jane Mokoena", "Nurse", 2, "Ward"},
        {"Sipho Dlamini", "Nurse", 2, "Ward"},
        {"Dr Priya Naidoo", "Doctor", 3, "Doctors"},
        {"Dr Mark Steyn", "Senior Doctor", 4, "Doctors"},
    };
    s.chain = {"Ann Botha", "Jane Mokoena", "Dr Priya Naidoo", "Dr Mark Steyn"};

    s.workflowName = "Patient admission";
    s.builder = "Sequential";
    s.stages = {
        {"Admission", {"Register patient", "Capture patient details"}},
        {"Assessment", {"Nurse assessment", "Doctor assessment", "Request tests"}},
        {"Emergency Care", {"Stabilise patient"}},
        {"Treatment", {"Determine treatment", "Obtain approval", "Perform treatment"}},
        {"Discharge", {"Monitor patient", "Discharge patient"}},
    };

    s.assignment = "RoleBased";
    s.rules = {
        {"requiredPosition", "Senior Doctor", "Senior Doctor"},
        {"requiredPosition", "Doctor", "Doctor"},
        {"requiredPosition", "Nurse", "Nurse"},
        {"requiredPosition", "Admission Clerk", "Admission Clerk"},
    };

    s.routing.kind = "EventBased";
    s.routing.normalStage = "Treatment";
    s.routing.emergencyStage = "Emergency Care";
    s.routing.metricKey = "triage score";
    s.routing.approvalLimit = 6;
    s.routing.standardStage = "Treatment";
    s.routing.escalationStage = "Emergency Care";
    s.routing.thresholds = {{8, "Emergency Care"}, {4, "Treatment"}, {0, "Assessment"}};

    s.startingData = {{"triage score", 3}};
    s.sampleWork = {
        {"Check vitals", "Nurse", "Low"},
        {"Order blood tests", "Doctor", "Medium"},
        {"Stabilise after collapse", "Senior Doctor", "Critical"},
        {"Update patient file", "Admission Clerk", "Low"},
    };
    return s;
}

ScenarioSetup Scenarios::insurance()
{
    ScenarioSetup s;
    s.name = "Insurance claims";
    s.summary = "A claim is lodged, assessed and settled. Claims above the approval limit go to senior review.";

    s.teams = {
        {"Claims Desk", "Claims", 1, ""},
        {"Assessors", "Assessment", 2, ""},
        {"Management", "Management", 4, ""},
    };
    s.people = {
        {"Lerato Khumalo", "Claims Clerk", 1, "Claims Desk"},
        {"Pieter van Wyk", "Assessor", 2, "Assessors"},
        {"Aisha Patel", "Assessor", 2, "Assessors"},
        {"Thabo Nkosi", "Senior Assessor", 3, "Assessors"},
        {"Karen Smit", "Claims Manager", 4, "Management"},
    };
    s.chain = {"Lerato Khumalo", "Pieter van Wyk", "Thabo Nkosi", "Karen Smit"};

    s.workflowName = "Insurance claim";
    s.builder = "Escalating";
    s.stages = {
        {"Lodgement", {"Capture claim", "Check policy cover"}},
        {"Assessment", {"Inspect damage", "Estimate repair cost"}},
        {"Senior Review", {"Review large claim"}},
        {"Settlement", {"Approve payout", "Pay claimant"}},
    };

    s.assignment = "RoleBased";
    s.rules = {
        {"requiredPosition", "Claims Manager", "Claims Manager"},
        {"requiredPosition", "Senior Assessor", "Senior Assessor"},
        {"requiredPosition", "Assessor", "Assessor"},
        {"requiredPosition", "Claims Clerk", "Claims Clerk"},
    };

    s.routing.kind = "ApprovalChain";
    s.routing.normalStage = "Assessment";
    s.routing.emergencyStage = "Senior Review";
    s.routing.metricKey = "claim amount";
    s.routing.approvalLimit = 50000;
    s.routing.standardStage = "Assessment";
    s.routing.escalationStage = "Senior Review";
    s.routing.thresholds = {{100000, "Senior Review"}, {20000, "Assessment"}, {0, "Lodgement"}};

    s.startingData = {{"claim amount", 12000}};
    s.sampleWork = {
        {"Photograph the damage", "Assessor", "Low"},
        {"Get a second quote", "Assessor", "Medium"},
        {"Sign off a large payout", "Claims Manager", "High"},
        {"Call the client back", "Claims Clerk", "Low"},
    };
    return s;
}

ScenarioSetup Scenarios::shield()
{
    ScenarioSetup s;
    s.name = "S.H.I.E.L.D. threat response";
    s.summary = "The original main.cpp mission. Severity decides whether the field team or the Avengers go.";

    s.teams = {
        {"Field Agents", "Agent", 1, ""},
        {"Command Centre", "Director", 2, ""},
        {"Tech Support", "Tech", 4, "Command Centre"},
    };
    s.people = {
        {"Phil Coulson", "Agent", 1, "Field Agents"},
        {"Natasha Romanoff", "Agent", 1, "Field Agents"},
        {"Maria Hill", "Deputy Director", 2, "Command Centre"},
        {"John Doe", "Head of I.T", 4, "Tech Support"},
        {"Nick Fury", "Director", 3, ""},
    };
    s.chain = {"Field Agents", "Command Centre", "Tech Support"};

    s.workflowName = "Threat Response";
    s.builder = "Sequential";
    s.stages = {
        {"Intel Gathering", {"Scan for threat", "Threat Assessment"}},
        {"Threat Assessment", {"Rate threat level"}},
        {"Deploy Team", {"Send field team"}},
        {"Assemble Avengers", {"Call Avengers"}},
        {"Mission Debrief", {"File mission report"}},
    };

    s.assignment = "RoleBased";
    s.rules = {
        {"requiredPosition", "Director", "Director"},
        {"requiredPosition", "Agent", "Agent"},
        {"requiredPosition", "Head of I.T", "Head of I.T"},
    };

    s.routing.kind = "SeverityBased";
    s.routing.normalStage = "Deploy Team";
    s.routing.emergencyStage = "Assemble Avengers";
    s.routing.metricKey = "threat level";
    s.routing.approvalLimit = 5;
    s.routing.standardStage = "Deploy Team";
    s.routing.escalationStage = "Assemble Avengers";
    s.routing.thresholds = {{8, "Assemble Avengers"}, {4, "Deploy Team"}, {0, "Threat Assessment"}};

    s.startingData = {{"threat level", 2}};
    s.sampleWork = {
        {"Scan the area", "Agent", "Low"},
        {"Secure the tesseract", "Agent", "Critical"},
        {"Brief the director", "Director", "Medium"},
        {"Patch the helicarrier", "Head of I.T", "High"},
    };
    return s;
}

std::vector<ScenarioSetup> Scenarios::all()
{
    return {hospital(), insurance(), shield()};
}
