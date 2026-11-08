#include <crow.h>
#include <services.h>

#define PORT 4242

/**
 * @brief POST /validate-cpf - Validates a CPF
 */
crow::json::wvalue validate_cpf(const crow::request& req) {
    crow::json::wvalue response;

    if (req.body.empty()) {
        response["status"] = false;
        response["reason"] = "Bad Request";
        return response;
    }

    crow::json::rvalue body_json = crow::json::load(req.body);
    if (!body_json.has("id")) {
        response["status"] = false;
        response["reason"] = "Malformed Request";
    }

    std::string doc = body_json["id"].s();
    auto valid_status = validate_document(doc);

    response["status"] = valid_status.first;
    response["reason"] = valid_status.second;
    return response;
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/validate-cpf")
        .methods(crow::HTTPMethod::POST)(validate_cpf);

    app.port(PORT).multithreaded().run();
}
