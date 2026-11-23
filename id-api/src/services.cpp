#include <services.h>

#include <algorithm>
#include <cctype> 
#include <string>
#include <utility>

bool is_numeric(const std::string& document) {
    for (std::size_t i = 0; i < document.size(); i++) {
        if (!(document[i] >= 48 && document[i] <= 57)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Validates a CPF document and returns a description of the operation
 */
std::pair<bool, std::string> validate_document(const std::string& document) {
    if (document.empty()) {
        return {false, "Empty Field"};
    }

    document.erase(
        std::remove_if(
            std::begin(document),
            std::end(document),
            [](unsigned char c) {
                return !std::isdigit(c);
            }
        ),
        std::end(document)
    );

    if (document.size() != 11 || !is_numeric(document)) {
        return {false, "Does not represent a CPF"};
    }

    // Check first digit identifier
    short sum = 0;
    for (std::size_t i = 0; i < document.size() - 2; i++) {
        short digit = document[i] - '0';
        sum += digit * (10 - i);
    }
    sum *= 10;

    short first_id_digit = document[9] - '0';
    if (!(sum % 11 == first_id_digit ||
          (sum % 11 == 10 && first_id_digit == 0))) {
        return {false, "Invalid first identifier digit"};
    }

    sum = 0;
    for (std::size_t i = 0; i < document.size() - 1; i++) {
        short digit = document[i] - '0';
        sum += digit * (11 - i);
    }
    sum *= 10;

    short second_id_digit = document[10] - '0';
    if (!(sum % 11 == second_id_digit ||
          (sum % 11 == 10 && second_id_digit == 0))) {
        return {false, "Invalid second identifier digit"};
    }

    return {true, "Valid Document"};
}