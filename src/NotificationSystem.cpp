/**
 * @file NotificationSystem.cpp
 * @brief Notification service (Adaptee).
 * @author Anke de Frey
 * @date 2026-10-22
 */

#include "NotificationSystem.h"
#include <iostream>

NotificationSystem::NotificationSystem(){
	this->sentCount = 0;
	this->lastMessage = "";
}

void NotificationSystem::sendNotification() {
	this->lastMessage = "Notification sent to all registered recipients";
	this->sentCount++;
	cout<<"[NotificationSystem] "<<this->lastMessage<<std::endl;
}
