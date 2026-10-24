/**
 * @file Completed.cpp
 * @brief Implementation of the Completed state.
 * @author Jezelle Govender
 * @date 2026-10-24
 */
#include "Completed.h"
#include "WorkflowInstance.h"
#include <iostream>

/**
 * @brief Completed is an end state.
 * @param ctx A pointer to the WorkflowInstance (Context) that is changing state.
 */
void Completed::handle(WorkflowInstance* ctx) {
	if(ctx == nullptr) {
		return;
	}

	std::cout << "[State] Workflow already completed, no further transitions\n";
}
