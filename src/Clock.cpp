/**
 * @file Clock.cpp
 * @brief Implementation of workflow event timestamps.
 * @author Anke de Frey
 * @date 2026-10-26
 */

#include "Clock.h"
#include <ctime>

std::string Clock::now() {
	std::time_t t = std::time(NULL);
	std::tm* lt = std::localtime(&t);
	char buffer[32];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", lt);
	return std::string(buffer);
}