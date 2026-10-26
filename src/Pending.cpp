/**
 * @file Pending.cpp
 * @brief Implementation of the Pending state.
 * @author Jezelle Govender
 * @date 2026-10-24
 */
#include "Pending.h"
#include "Active.h"
#include "WorkflowInstance.h"
#include <iostream>

/**
 * @brief Pending to Active. A pending workflow is started the first time it is handled.
 * @param ctx A pointer to the WorkflowInstance (Context) that is changing state.
 */
void Pending::handle(WorkflowInstance* ctx) {
	if(ctx == nullptr) {
		return;
	}

	std::cout << "[State] Pending -> Active\n";
	ctx->setState(new Active()); 
}

std::string Pending::getName() const {
	return "Pending";
}

