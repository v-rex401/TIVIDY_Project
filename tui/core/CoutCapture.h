/**
 * @file CoutCapture.h
 * @brief Catches what the engine prints to std::cout while one call runs.
 * @author JP
 * @date 2026-10-31
 */

#ifndef COUTCAPTURE_H
#define COUTCAPTURE_H

#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

/**
 * @brief Redirects std::cout into a buffer for as long as it lives.
 *
 * Several engine classes (the states, observers, visitors) report by
 * printing to std::cout. The TUI draws the screen through std::cout too, so
 * every engine call is wrapped in one of these: the output is kept and shown
 * in the engine panel instead of tearing up the screen.
 *
 * Captures nest: an inner capture restores the outer one when it ends.
 */
class CoutCapture
{
public:
    /** @brief Start capturing std::cout. */
    CoutCapture();

    /** @brief Stop capturing and give std::cout its previous buffer back. */
    ~CoutCapture();

    /**
     * @brief Everything printed so far.
     * @return The raw text.
     */
    std::string text() const;

    /**
     * @brief Everything printed so far, split into lines.
     * @return The non-empty lines, without their line endings.
     */
    std::vector<std::string> lines() const;

    CoutCapture(const CoutCapture &) = delete;            ///< Not copyable: owns the redirection.
    CoutCapture &operator=(const CoutCapture &) = delete; ///< Not copyable: owns the redirection.

private:
    std::ostringstream buffer; ///< Receives the output.
    std::streambuf *previous;  ///< std::cout's buffer before this capture.
};

#endif
