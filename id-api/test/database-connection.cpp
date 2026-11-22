#include <conncpp.hpp>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "conncpp/Driver.hpp"
#include "conncpp/Properties.hpp"
#include "conncpp/ResultSetMetaData.hpp"

void print_vector(const std::vector<std::string>& arr) {
    for (const auto& str : arr) {
        std::cout << str << ',';
    }

    std::cout << '\n';
}

int main() {
    std::string sample_str = std::getenv("API_VERSION");
    std::cout << "Currently running on: " << sample_str << '\n';
    
    sql::Driver* driver = sql::mariadb::get_driver_instance();
    sql::Properties properties({{"user", "api"},
                                {"password", "password"},
                                {"hostName", "api-db"},
                                {"schema", "lambda"}});

    std::unique_ptr<sql::Connection> conn(driver->connect(properties));
    std::shared_ptr<sql::Statement> statement(conn->createStatement());

    std::shared_ptr<sql::ResultSet> result(
        statement->executeQuery("SELECT * FROM request;"));
    auto result_metadata =
        std::shared_ptr<sql::ResultSetMetaData>(result->getMetaData());

    std::vector<std::string> cols_names;
    for (uint32_t i = 1; i <= result_metadata->getColumnCount(); i++) {
        cols_names.emplace_back(result_metadata->getColumnName(i));
    }

    std::vector<std::vector<std::string>> rows;

    uint32_t c = 0;
    while (result->next()) {
        std::vector<std::string> temp;
        for (uint32_t i = 1; i <= result_metadata->getColumnCount(); i++) {
            temp.emplace_back(std::string(result->getString(i)));
        }
        rows.push_back(std::move(temp));
        c++;
    }
 
    for (const auto& row : rows) {
        print_vector(row);
    }
}