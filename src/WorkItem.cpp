/**
 * @file WorkItem.cpp
 * @brief Implementation of the WorkItem class
 * @author Kayla Falconer
 * @date 2026-10-24
 */

#include "WorkItem.h"
#include "WorkflowVisitor.h"

/**
 * @brief Sets the next WorkItem in the Chain of Responsibility
 */
void WorkItem::setNextHandler(WorkItem* next) {
	this->successor = next;
}

/**
 * @brief Accepts a visitor to perform an operation on the WorkItem
 */
void WorkItem::accept(WorkflowVisitor* v)
{
    if(v != nullptr)
    {
        v->visitWorkItem(this);
    }
}