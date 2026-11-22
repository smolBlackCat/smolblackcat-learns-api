#include <crow.h>
#include <crow/app.h>
#include <crow/common.h>
#include <crow/json.h>
#include <services.h>

#include <conncpp.hpp>
#include <conncpp/Properties.hpp>
#include <conncpp/ResultSet.hpp>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "conncpp/Exception.hpp"
#include "conncpp/PreparedStatement.hpp"

using DocumentStatus = std::tuple<std::string, bool>;

/**
 * @brief API's database session
 *
 * It provides an interface for fetching records from the database that are
 * relevant to the API's functionality
 */
class DatabaseSession {
public:
    const static int PAGE_SIZE = 10;

    DatabaseSession() {
        driver = sql::mariadb::get_driver_instance();

        if (!driver) {
            throw std::runtime_error{
                "It was not possible to instantiate a database session"};
        }
    }

    /**
     * @brief Retrieve from the database a page of logged documents.
     *
     * It is important to note that each page has exactly 10 documents. In that
     * case, if there are 35 documents in the db, therefore there are 4 pages
     */
    std::vector<DocumentStatus> get_document_status_page(int page = 1) {
        // Get the number of records in requests
        std::unique_ptr<sql::Connection> cnn{driver->connect(properties)};
        if (!cnn) {
            std::cout << "Something went wrong. It was not possible to "
                         "establish a connection"
                      << '\n';
            return {};
        }

        std::unique_ptr<sql::Statement> count_stat{cnn->createStatement()};

        std::ostringstream query;
        query << "SELECT document, valid FROM request LIMIT 10 OFFSET ";
        query << 10 * (page - 1);
        std::unique_ptr<sql::ResultSet> records_page{
            count_stat->executeQuery(query.str())};

        std::vector<DocumentStatus> docs_page;
        docs_page.reserve(10);

        while (records_page->next()) {
            docs_page.emplace_back(DocumentStatus{records_page->getString(1),
                                                  records_page->getBoolean(2)});
        }

        // Fetch the requested page
        return docs_page;
    }

    /**
     * Registers a new document into the database
     */
    void log_document_status(const std::string& document, bool valid_status) {
        std::unique_ptr<sql::Connection> cnn{driver->connect(properties)};
        if (!cnn) {
            std::cout << "Something went wrong. It was not possible to "
                         "establish a connection"
                      << '\n';
            return;
        }
        std::unique_ptr<sql::PreparedStatement> statement{cnn->prepareStatement(
            "INSERT INTO request (document, valid) VALUES (?, ?)")};

        try {
            statement->setString(1, document);
            statement->setBoolean(2, valid_status);

            statement->executeUpdate();
        } catch (sql::SQLException& e) {
            std::cerr << "Error contacting the database: " << e.what() << '\n';
        }
        return;
    }

private:
    sql::Driver* driver;
    // TODO: Hardcoded Database Connection Settings
    sql::Properties properties{{{"user", "api"},
                                {"hostName", "database"},
                                {"password", "lotus-waters"},
                                {"schema", "lambda"}}};
};

/**
 * @brief Government API for handling document validation
 */
class API {
public:
    const static int PORT = 4242;

    API() {
        CROW_ROUTE(app, "/validate-cpf")
            .methods(crow::HTTPMethod::POST)(
                [this](const crow::request& req) { return validate_cpf(req); });

        CROW_ROUTE(app, "/records/<int>")
            .methods(crow::HTTPMethod::GET)([this](int page) {
                auto records_page = db_session.get_document_status_page(page);
                crow::json::wvalue response_body;

                std::vector<crow::json::wvalue> records;
                for (const auto [document, valid] : records_page) {
                    crow::json::wvalue cur_record;
                    cur_record["document"] = document;
                    cur_record["valid"] = valid;
                    records.push_back(std::move(cur_record));
                }
                response_body["records"] = std::move(records);

                return response_body;
            });

        CROW_ROUTE(app, "/debug-endpoint")
            .methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
                auto copy = req;
                return 200;
            });
    }

    /**
     * @brief POST /validate-cpf - Validates a CPF
     */
    crow::response validate_cpf(const crow::request& req) {
        crow::response response;
        crow::json::wvalue response_body;

        if (req.body.empty()) {
            response_body["status"] = false;
            response_body["reason"] = "Bad Request";

            response.code = 400;
            response.body = std::string(response_body);

            return response_body;
        }

        crow::json::rvalue body_json = crow::json::load(req.body);
        if (!body_json.has("id")) {
            response_body["status"] = false;
            response_body["reason"] = "Malformed Request";

            response.code = 400;
            response.body = std::string(response_body);

            return response;
        }

        std::string doc = body_json["id"].s();
        auto valid_status = validate_document(doc);

        response_body["status"] = valid_status.first;
        response_body["reason"] = valid_status.second;

        db_session.log_document_status(doc, valid_status.first);

        response.code = 200;
        response.body = std::string(response_body);

        return response;
    }

    /**
     * @brief GET /records?page={n} - Obtain the nth 10 entries from the log
     * system
     */
    crow::response get_records_page(const crow::request& req) { return {}; }

    void run() { app.port(PORT).multithreaded().run(); }

private:
    crow::SimpleApp app;
    DatabaseSession db_session;
};

int main() {
    API api_server;

    api_server.run();
}
