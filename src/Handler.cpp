#include "Handler.h"

std::string Handler::getName()
{
	return this->name;
}

std::string Handler::getTitle()
{
	return this->title;
}

void Handler::printHierarchy(std::string /* indent */)
{
	// TODO - implement Handler::printHierarchy
}

void Handler::addMember(int /* Component */)
{
	// TODO - implement Handler::addMember
}

void Handler::removeMember(int /* Component */)
{
	// TODO - implement Handler::removeMember
}

void Handler::setNextHandler(Handler * /* next */)
{
	// TODO - implement Handler::setNextHandler
}

void Handler::handleRequest()
{
	// TODO - implement Handler::handleRequest
}
