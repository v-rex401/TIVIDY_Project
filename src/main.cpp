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
    Stage *intel = new Stage("Intel Gathering");
    Stage *assessment = new Stage("Threat Assessment");
    Stage *deploy = new Stage("Deploy Team");
    Stage *emergency = new Stage("Assemble Avengers");
    Stage *debrief = new Stage("Mission Debrief");

    //*Create the tasks
    intel->addMember(new Task("Scan for threat"));

    //*Make the builder then instantiate NickFury(director)

    //*Set up external connections

    //*Choose the rules (assignments)

    //*Create workflow manager

    //*Create the facade

    //*Undo Mechanisms

    return 0;
}