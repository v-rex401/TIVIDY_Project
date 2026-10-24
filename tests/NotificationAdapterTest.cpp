/**
 * @file NotificationAdapterTest.cpp
 * @brief Behaviour tests for the Adapter pattern participants.
 * @author Anke de Frey
 * @date 2026-10-23
 */

#include <gtest/gtest.h>
#include "ExternalSystem.h"
#include "NotificationAdapter.h"
#include "NotificationSystem.h"

TEST(NotificationSystemTest, StartsWithNothingSent) {
    NotificationSystem service;
    EXPECT_EQ(0, service.getSentCount());
    EXPECT_EQ("", service.getLastMessage());
}

TEST(NotificationSystemTest, SendNotificationRecordsTheSend) {
    NotificationSystem service;
    service.sendNotification();
    EXPECT_EQ(1, service.getSentCount());
    EXPECT_FALSE(service.getLastMessage().empty());
}

TEST(NotificationAdapterTest, RequestReachesTheAdaptee) {
    NotificationSystem service;
    NotificationAdapter adapter(&service);

    adapter.request();

    EXPECT_EQ(1, service.getSentCount());
}

TEST(NotificationAdapterTest, WorksThroughTheTargetInterface) {
    NotificationSystem service;
    NotificationAdapter adapter(&service);
    ExternalSystem* target = &adapter;

    target->request();
    target->request();

    EXPECT_EQ(2, service.getSentCount());
}

TEST(NotificationAdapterTest, EachAdapteeIsIndependent) {
    NotificationSystem a;
    NotificationSystem b;
    NotificationAdapter adapterA(&a);
    NotificationAdapter adapterB(&b);

    adapterA.request();

    EXPECT_EQ(1, a.getSentCount());
    EXPECT_EQ(0, b.getSentCount());
}

TEST(NotificationAdapterTest, NullAdapteeIsSafe) {
    NotificationAdapter adapter(NULL);
    EXPECT_NO_THROW(adapter.request());
}