#include <services.h>

#include <catch2/catch_test_macros.hpp>
#include <utility>

TEST_CASE("[''] -> {false, \"Empty Field\"}") {
    std::string doc = "";
    std::pair<bool, std::string> output{false, "Empty Field"};

    CHECK(validate_document(doc) == output);
}

TEST_CASE("['dsascvfrewed12'] -> {false, \"Does not represent a CPF\"}") {
    std::string doc = "dsascvfrewed12";
    std::pair<bool, std::string> output{false, "Does not represent a CPF"};

    CHECK(validate_document(doc) == output);
}

TEST_CASE("['60808704090'] -> {false, \"Invalid first identifier digit\"}") {
    std::string doc = "60808704090";
    std::pair<bool, std::string> output{false,
                                        "Invalid first identifier digit"};

    CHECK(validate_document(doc) == output);
}

TEST_CASE("['60808704019'] -> {false, \"Invalid second identifier digit\"}") {
    std::string doc = "60808704019";
    std::pair<bool, std::string> output{false,
                                        "Invalid second identifier digit"};

    CHECK(validate_document(doc) == output);
}

TEST_CASE("['60808704010'] -> {true, \"Valid Document\"}") {
    std::string doc = "60808704010";
    std::pair<bool, std::string> output{true, "Valid Document"};

    CHECK(validate_document(doc) == output);
}