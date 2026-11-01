/**
 * @file TeamTest.cpp
 * @brief Tests for the Team composite's member list.
 * @author JP
 * @date 2026-10-31
 */

#include <gtest/gtest.h>

#include "Employee.h"
#include "Team.h"

TEST(TeamTest, NewTeamHasNoMembers)
{
    Team team("Ward", "Nurses", 2);
    EXPECT_TRUE(team.getMembers().empty());
}

TEST(TeamTest, MembersComeBackInTheOrderTheyWereAdded)
{
    Team team("Ward", "Nurses", 2);
    Employee *jane = new Employee("Jane", "Nurse", 2);
    Employee *sipho = new Employee("Sipho", "Nurse", 2);
    team.addMember(jane);
    team.addMember(sipho);

    ASSERT_EQ(team.getMembers().size(), 2u);
    EXPECT_EQ(team.getMembers()[0], jane);
    EXPECT_EQ(team.getMembers()[1], sipho);
}

TEST(TeamTest, AddingTheSameMemberTwiceKeepsOneCopy)
{
    Team team("Ward", "Nurses", 2);
    Employee *jane = new Employee("Jane", "Nurse", 2);
    team.addMember(jane);
    team.addMember(jane);

    EXPECT_EQ(team.getMembers().size(), 1u);
}

TEST(TeamTest, NestedTeamIsOneMember)
{
    Team hospital("Hospital", "Staff", 1);
    Team *ward = new Team("Ward", "Nurses", 2);
    ward->addMember(new Employee("Jane", "Nurse", 2));
    hospital.addMember(ward);

    ASSERT_EQ(hospital.getMembers().size(), 1u);
    EXPECT_EQ(hospital.getMembers()[0]->getName(), "Ward");
}
