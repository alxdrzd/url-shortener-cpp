#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include "Database.h"
#include "UrlRepository.h"
#include "HttpServer.h"

int main() {
    const char *env_conn_str = std::getenv("DB_CONN_STR");
    std::string basic_conn_str = "host=localhost port=5432 user=postgres password=secret dbname=url_shortener_db";
    std::string conn_str = env_conn_str ? env_conn_str : basic_conn_str;

    std::cout << "Connecting to: " << conn_str << std::endl;

    std::unique_ptr<Database> db;
    int attempts = 15;
    while (attempts > 0) {
        try {
            db = std::make_unique<Database>(conn_str);
            break;
        } catch (const std::exception &e) {
            attempts--;
            std::cerr << "Waiting for database... (" << attempts << " attempts left). Error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    if (!db) {
        std::cerr << "Could not connect to database. Exiting." << std::endl;
        return 1;
    }
    UrlRepository repo(*db);

    try {
        HttpServer server("0.0.0.0", 8080, repo, 4, "./static");

        server.run();
    } catch (const std::exception &e) {
        std::cerr << "Server fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
