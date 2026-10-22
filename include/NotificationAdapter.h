#ifndef NOTIFICATIONADAPTER_H
#define NOTIFICATIONADAPTER_H

class NotificationAdapter : ExternalSystem {

public:
	NotificationSystem* adaptee;

	void request();
};

#endif
