#include <iostream>
#include "WorkflowBuilder.h"
#include "SequentialWorkflowBuilder.h"
#include "EscalatingWorkflowBuilder.h"
#include "WorkflowDirector.h"
#include "Stage.h"
#include "WorkItem.h"
#include "Team.h"
#include "Employee.h"
#include "Task.h"
#include "Handler.h"
#include "RoleBasedAssignment.h"
#include "SeverityBasedRouting.h"
#include "WorkflowManager.h"
#include "WorkflowFacade.h"
#include "AuditLogObserver.h"
#include "WorkflowDefinition.h"

//? Note that some of this functionality will be moving to facade
int main()
{
    //* Create the Director
    WorkflowDirector *NickFury;

    /*
     * Staff Comprising of:
     * Employees (Leaves)
     * Teams:
     *  - fieldTeam (Phil, Natasha)
     *  - command (Maria Hill, techSupport(John Doe)
     */
    Team *fieldTeam = new Team("Field Agents", "Agent", 1);
    fieldTeam->addMember(new Employee("Phil Coulson", "Agent", 1));
    fieldTeam->addMember(new Employee("Natasha Romanoff", "Agent", 1));

    Team *command = new Team("Command Centre", "Director", 2);
    command->addMember(new Employee("Maria Hill", "Deputy Director", 2));

    Team *techSupport = new Team("Tech Support", "Tech", 4);
    techSupport->addMember(new Employee("John Doe", "Head of I.T", 4));

    command->addMember(techSupport);

    //*Set up the chain of Responsibility

    //* fieldTeam -> command -> techSupport
    fieldTeam->setNextHandler(command);
    command->setNextHandler(techSupport);

    //* Make the Stages
    std::vector<std::string> stages;
    stages.push_back("Intel Gathering");
    stages.push_back("Threat Assessment");
    stages.push_back("Deploy Team");
    stages.push_back("Assemble Avengers");
    stages.push_back("Mission Debrief");

    //*Create the tasks that can be used in a workflow
    std::map<std::string, std::vector<std::string>> tasks;
    std::vector<std::string> intelTasks;
    intelTasks.push_back("Scan for threat");
    intelTasks.push_back("Threat Assessment");

    tasks.insert({"Intel Gathering", intelTasks});

    std::vector<std::string> assessTasks;
    assessTasks.push_back("Rate threat level");
    tasks.insert({"Threat Assessment", assessTasks});

    std::vector<std::string> deployTasks;
    deployTasks.push_back("Send field team");
    tasks.insert({"Deploy Team", deployTasks});

    std::vector<std::string> emergencyTasks;
    emergencyTasks.push_back("Call Avengers");
    tasks.insert({"Assemble Avengers", emergencyTasks});

    std::vector<std::string> debriefTasks;
    debriefTasks.push_back("File mission report");
    tasks.insert({"Mission Debrief", debriefTasks});

    //*Build the definition by which our workflows will work
    SequentialWorkflowBuilder *buildThreat = new SequentialWorkflowBuilder();
    NickFury = new WorkflowDirector(buildThreat);

    //*Make the builder then instantiate NickFury(director)
    NickFury->construct("Threat Response", stages, tasks);

    buildThreat->setAssignmentStrategyType("RoleBased");
    buildThreat->setRoutingStrategyType("SeverityBased");

    //*We now use builder to get our product -> WorkflowDefinition
    WorkflowDefinition *threatResponse = buildThreat->getWorkflowDefinition();

    std::vector<Staff *> pool;
    pool.push_back(new Employee("Phil Coulson", "Agent", 1));
    pool.push_back(new Employee("Maria Hill", "Analyst", 2));
    pool.push_back(new Employee("Nick Fury", "Director", 3));
    RoleBasedAssignment *assign = new RoleBasedAssignment(pool);

    //* Create workflow manager + facade
    WorkflowFacade facade;
    facade.addDefinition(threatResponse); // facade now owns it

    //?Need to get the stages from workflow defintion that I created
    facade.setAssignmentStrategy(assign);
    facade.createWorkflowInstance(threatResponse);

    //?The User would have to select which stages they created they want to use to set up the routing
    facade.setRoutingStrategy(new SeverityBasedRouting("Deploy Team", "Assemble Avengers"));

    //* Run it
    AuditLogObserver audit;
    WorkflowInstance *mission = facade.createWorkflowInstance(threatResponse);
    mission->attach(&audit);
    facade.executeWorkflow(mission);

    std::cout << mission->getId() << " is " << mission->getState()->getName() << std::endl;

    //* Undo Mechanisms
    // skip for now

    delete NickFury;

    return 0;
}