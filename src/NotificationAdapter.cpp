/**
 * @file NotificationAdapter.cpp
 * @brief Implementation of the Adapter wrapping NotificationSystem.
 * @author Anke de Frey
 * @date 2026-10-22
 */

#include "NotificationAdapter.h"

NotificationAdapter::NotificationAdapter(NotificationSystem* adaptee) {
    this->adaptee = adaptee;
}

NotificationAdapter::~NotificationAdapter() {
	this->adaptee = NULL;
}

void NotificationAdapter::request() {
	if(this->adaptee == NULL){
		return;
	}
	this->adaptee->sendNotification();
}
