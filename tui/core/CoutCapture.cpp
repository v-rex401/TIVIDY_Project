/**
 * @file CoutCapture.cpp
 * @brief Implementation of CoutCapture.
 * @author JP
 * @date 2026-10-31
 */

#include "CoutCapture.h"

#include <iostream>

CoutCapture::CoutCapture() : previous(std::cout.rdbuf(buffer.rdbuf())) {}

CoutCapture::~CoutCapture()
{
    std::cout.rdbuf(previous);
}

std::string CoutCapture::text() const
{
    return buffer.str();
}

std::vector<std::string> CoutCapture::lines() const
{
    std::vector<std::string> result;
    std::istringstream in(buffer.str());
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        if (!line.empty())
        {
            result.push_back(line);
        }
    }
    return result;
}
