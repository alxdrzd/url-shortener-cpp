#include <iostream>
#include "Database.h"
#include "UrlRepository.h"

int main() {
    try {
        std::string conn_str = "host=localhost port=5432 user=postgres password=secret dbname=postgres";
        Database db(conn_str);
        UrlRepository repo(db);

        // 1. Тестируем сохранение
        std::string my_long_url = "https://github.com/alxdrzd/super-project-link";
        std::string key = repo.save_url(my_long_url, 1); // 1 - это ID нашего 'admin'

        std::cout << "Saved! Short key: " << key << std::endl;

        // 2. Тестируем поиск
        auto found_url = repo.get_original_url(key);
        if (found_url) {
            std::cout << "Found original: " << *found_url << std::endl;
        } else {
            std::cout << "URL not found!" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}