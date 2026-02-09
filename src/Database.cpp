#include "Database.h"
#include <iostream>

Database::Database(const std::string &connection_string) {
    try {
        m_connection = std::make_unique<pqxx::connection>(connection_string);

        if (m_connection->is_open()) {
            std::cout << "[Database] Successfully connected to : " << m_connection->dbname() << std::endl;
        } else {
            std::cout << "[Database] Could NOT oped the DB" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "[Database] Connection error : " << e.what() << std::endl;
        throw;
    }
}

pqxx::connection &Database::get_connection() const {
    return *m_connection;
}
