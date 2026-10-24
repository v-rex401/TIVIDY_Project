/**
 * @file Client.h
 * @brief Text user interface (TUI) that drives the workflow engine through the WorkflowFacade.
 * @author JP
 * @date 2026-10-24
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <vector>

class WorkflowFacade;

/**
 * @brief Client participant of the Facade pattern.
 *
 * Runs the menu loop: shows numbered menus, reads and validates the
 * user's choice, re-prompts on bad input and stops cleanly on "Exit" or
 * end of input. It holds no workflow logic; everything the engine does
 * goes through the WorkflowFacade (UML-019).
 *
 * Input and output are streams so the loop can be driven from tests.
 */
class Client {

private:
	WorkflowFacade* facade; ///< Entry point to the engine (not owned).
	std::istream& in;       ///< Where the user's input is read from.
	std::ostream& out;      ///< Where menus and messages are written.
	bool running;           ///< False once the user exits or input ends.

	/**
	 * @brief Show a numbered menu and read a valid choice.
	 *
	 * Options are numbered from 1; 0 is always the way out of the menu.
	 * Anything that is not a whole number in range is rejected and the
	 * menu is shown again.
	 *
	 * @param title Heading printed above the options.
	 * @param options Labels for choices 1..n.
	 * @param exitLabel Label for choice 0, e.g. "Back" or "Exit".
	 * @return The chosen number (0..n), or -1 if input ended.
	 */
	int choose(const std::string& title, const std::vector<std::string>& options, const std::string& exitLabel);

	/**
	 * @brief Read one line of input, without surrounding whitespace.
	 * @param line Receives the trimmed line.
	 * @return False if input ended; @c running is cleared too.
	 */
	bool readLine(std::string& line);

	/**
	 * @brief Parse a whole number in [min, max].
	 * @param text The text to parse.
	 * @param min Smallest accepted value.
	 * @param max Largest accepted value.
	 * @param value Receives the number when the text is valid.
	 * @return True if @p text is a whole number in range.
	 */
	static bool parseChoice(const std::string& text, int min, int max, int& value);

	/** @brief Top-level menu: start a workflow, open an instance or exit. */
	void mainMenu();

	/** @brief Menu for one workflow instance: work, actions, history, report. */
	void instanceMenu();

public:
	/**
	 * @brief Create a TUI bound to a facade and a pair of streams.
	 * @param facade The facade every engine call goes through (not owned).
	 * @param in Input stream; defaults to standard input.
	 * @param out Output stream; defaults to standard output.
	 */
	explicit Client(WorkflowFacade* facade, std::istream& in = std::cin, std::ostream& out = std::cout);

	/**
	 * @brief Run the menu loop until the user exits or input ends.
	 */
	void run();

	Client(const Client&) = delete;            ///< Not copyable: holds stream references.
	Client& operator=(const Client&) = delete; ///< Not copyable: holds stream references.
};

#endif // CLIENT_H
