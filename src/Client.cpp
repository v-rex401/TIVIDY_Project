/**
 * @file Client.cpp
 * @brief Implementation of the Client menu loop and input validation.
 * @author JP
 * @date 2026-10-24
 */

#include "Client.h"

#include <cctype>
#include <cstddef>

Client::Client(WorkflowFacade* facade, std::istream& in, std::ostream& out)
	: facade(facade), in(in), out(out), running(true) {
	// The menus don't call the engine yet; this keeps -Wunused-private-field quiet until they do.
	(void)this->facade;
}

void Client::run() {
	running = true;
	while (running) {
		mainMenu();
	}
	out << "Goodbye." << std::endl;
}

void Client::mainMenu() {
	std::vector<std::string> options;
	options.push_back("Start a workflow");
	options.push_back("Open a workflow instance");

	switch (choose("TIVIDY", options, "Exit")) {
	case 1:
		// Starting an instance through the facade is wired in with the definitions list.
		break;
	case 2:
		instanceMenu();
		break;
	default: // 0 = Exit, -1 = end of input
		running = false;
		break;
	}
}

void Client::instanceMenu() {
	std::vector<std::string> options;
	options.push_back("View current work");
	options.push_back("Take an action");
	options.push_back("View history");
	options.push_back("Run a report");

	// Each choice comes back to this menu until the user goes back.
	while (running) {
		int choice = choose("Workflow instance", options, "Back");
		if (choice <= 0) {
			return;
		}
	}
}

int Client::choose(const std::string& title, const std::vector<std::string>& options, const std::string& exitLabel) {
	const int max = static_cast<int>(options.size());

	while (true) {
		out << std::endl << "== " << title << " ==" << std::endl;
		for (std::size_t i = 0; i < options.size(); ++i) {
			out << "  " << (i + 1) << ". " << options[i] << std::endl;
		}
		out << "  0. " << exitLabel << std::endl;
		out << "> " << std::flush;

		std::string line;
		if (!readLine(line)) {
			out << std::endl;
			return -1;
		}

		int choice;
		if (parseChoice(line, 0, max, choice)) {
			return choice;
		}
		out << "Invalid choice. Enter a number from 0 to " << max << "." << std::endl;
	}
}

bool Client::readLine(std::string& line) {
	if (!std::getline(in, line)) {
		running = false;
		return false;
	}

	std::size_t first = 0;
	while (first < line.size() && std::isspace(static_cast<unsigned char>(line[first]))) {
		++first;
	}
	std::size_t last = line.size();
	while (last > first && std::isspace(static_cast<unsigned char>(line[last - 1]))) {
		--last;
	}
	line = line.substr(first, last - first);
	return true;
}

bool Client::parseChoice(const std::string& text, int min, int max, int& value) {
	// Cap the length so a long run of digits can't overflow int.
	if (text.empty() || text.size() > 9) {
		return false;
	}
	for (std::size_t i = 0; i < text.size(); ++i) {
		if (!std::isdigit(static_cast<unsigned char>(text[i]))) {
			return false;
		}
	}

	int parsed = 0;
	for (std::size_t i = 0; i < text.size(); ++i) {
		parsed = parsed * 10 + (text[i] - '0');
	}
	if (parsed < min || parsed > max) {
		return false;
	}
	value = parsed;
	return true;
}
