#include <iostream>
#include "Database.h"
#include "UrlRepository.h"
#include "HttpServer.h"

std::string conn_str = "host=localhost port=5432 user=postgres password=secret dbname=postgres";

int main() {
    try {
        // ...
        Database db(conn_str);
        UrlRepository repo(db);

        HttpServer server("0.0.0.0", 8080, repo, 4, "./static");

        server.run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}