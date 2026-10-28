#include <iostream>
#include "WorkflowBuilder.h"
#include "SequentialWorkflowBuilder.h"
#include "EscalatingWorkflowBuilder.h"
#include "WorkflowDirector.h"
#include "Stage.h"
#include "WorkItem.h"
#include "Team.h"
#include "Employee.h"

int main()
{
    //* Create the Director
    WorkflowDirector *NickFury;

    /**
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
    //*Make the builder then instantiate NickFury(director)

    //*Create the tasks

    //* Make the Teams / Employees

    //*Set up external connections

    //*Choose the rules (assignments)
    //* Create the Stages
    Stage *review = new Stage("Planning");
    Stage *draft = new Stage("Draft");
    Stage *last = new Stage("Final");

    //*Create workflow manager

    //*Create the facade

    //*Undo Mechanisms

    return 0;
}