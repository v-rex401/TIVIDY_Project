#ifndef NOTIFICATIONSYSTEM_H
#define NOTIFICATIONSYSTEM_H

#include <string>

using namespace std;

/**
 * @brief Adaptee of the Adapter pattern.
 *
 * Its interface is deliberately incompatible with ExternalSystem: that mismatch
 * is the problem the Adapter exists to solve.
 */

class NotificationSystem {

private:
		int sentCount; ///< how many notifications this service has sent
		string lastMessage; ///< text of the most recent notification

public:
	NotificationSystem();
	/** @brief The vendor's own call. Note the name differs from ExternalSystem::request(). */
	void sendNotification();
};

#endif
