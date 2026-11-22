#pragma once

#include <string>
#include <utility>

/**
 * @brief Validates a Brazilian CPF document
 *
 * @return The validation status and a detailed reason description
 */
std::pair<bool, std::string> validate_document(const std::string& document);