#ifndef NOTIFICATIONSYSTEM_H
#define NOTIFICATIONSYSTEM_H

/**
 * @brief Adaptee of the Adapter pattern.
 *
 * Its interface is deliberately incompatible with ExternalSystem: that mismatch
 * is the problem the Adapter exists to solve.
 */

class NotificationSystem {


public:
	/** @brief The vendor's own call. Note the name differs from ExternalSystem::request(). */
	void sendNotification();
};

#endif
