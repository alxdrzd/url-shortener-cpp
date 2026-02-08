#include "Database.h"
#include "URLEncoder.h"
#include <iostream>


int main() {
    std::string conn_str = "host=localhost port=5432 user=postgres password=secret dbname=postgres";

    try {
        Database db(conn_str);

        std::cout << "Database class works!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in main" << e.what() << std::endl;
    }

    std::uint64_t test_id = 100500;
    std::string code = URLEncoder::encode(test_id);

    std::cout << "ID: " << test_id << " -> Code: " << code << std::endl;

    // Попробуем очень большое число
    std::uint64_t big_id = 999999999;
    std::cout << "ID: " << big_id << " -> Code: " << URLEncoder::encode(big_id) << std::endl;

    return 0;
}