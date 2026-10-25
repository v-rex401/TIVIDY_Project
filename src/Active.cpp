/**
 * @file Active.cpp
 * @brief Implementation of the Active state.
 * @author Jezelle Govender
 * @date 2026-10-24
 */

#include "Active.h"
#include "Completed.h"
#include "WorkflowInstance.h"
#include <iostream>

/**
 * @brief From Active to Completed. An active workflow that finishes when it is handled.
 * @param ctx A pointer to the WorkflowInstance (Context) that is changing state.
 */
void Active::handle(WorkflowInstance* ctx) {
	if(ctx == nullptr) {
		return;
	}

	std::cout << "[State] Active -> Completed\n";
	ctx->setState(new Completed()); //the instance takes ownership now.
}
