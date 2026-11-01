/**
 * @file Cancelled.cpp
 * @brief Implementation of the Cancelled state.
 * @author Jezelle Govender
 * @date 2026-10-24
 */

#include "Cancelled.h"
#include "WorkflowInstance.h"
#include <iostream>

/**
 * @brief Cancelled is an end state.
 * @param ctx A pointer to the WorkflowInstance (Context) that is changing state.
 */
void Cancelled::handle(WorkflowInstance* ctx) {
	if(ctx == nullptr) {
		return;
	}

	std::cout << "[State] Workflow cancelled, no further transitions\n";
}

std::string Cancelled::getName() const {
	return "Cancelled";
}
