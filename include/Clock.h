#ifndef CLOCK_H
#define CLOCK_H

#include <string>

/**
 * @brief Supplies timestamps for workflow events.
 *
 * Kept separate so every event is stamped the same way and time formatting is
 * not scattered through the domain classes.
 */
class Clock {

public:
	/**
	 * @brief The current local time.
	 * @return Timestamp formatted as YYYY-MM-DD HH:MM:SS.
	 */
	static std::string now();
};

#endif