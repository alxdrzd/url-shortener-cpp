#include <iostream>
#include "Database.h"
#include "UrlRepository.h"
#include "HttpServer.h"
int main() {
    try {
        // 1. База
        std::string conn_str = "host=localhost port=5432 user=postgres password=secret dbname=postgres";
        Database db(conn_str);
        UrlRepository repo(db);

        // 2. Сервер
        // Запускаем на всех интерфейсах (0.0.0.0), порт 8080, 4 потока
        HttpServer server("0.0.0.0", 8080, repo, 4);

        // 3. Поехали!
        server.run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}