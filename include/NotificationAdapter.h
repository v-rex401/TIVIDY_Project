#ifndef NOTIFICATIONADAPTER_H
#define NOTIFICATIONADAPTER_H

#include "ExternalSystem.h"
#include "NotificationSystem.h"

/**
 * @brief Adapter of the Adapter pattern, object adapter, by composition.
 *
 * Presents NotificationSystem through the ExternalSystem interface the engine
 * expects, translating request() into the adaptee's sendNotification().
 * Inherits ExternalSystem publicly so the engine can hold it as ExternalSystem*
 */

class NotificationAdapter : public ExternalSystem {

private:
	NotificationSystem* adaptee;
public:
	/**
	 * @brief Wrap an existing notification service.
	 * @param adaptee The service to delegate to.
	 * Not owned, the caller keeps ownership and must outlive this adapter.
	 */
	NotificationAdapter(NotificationSystem* adaptee);

	virtual ~NotificationAdapter();

	/** @brief Delegates to NotificationSystem::sendNotification(). */
	virtual void request();
};

#endif
