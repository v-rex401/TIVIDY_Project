#include "WorkItem.h"

void WorkItem::setNextHandler(WorkItem* next) {
	this->successor = next;
}
