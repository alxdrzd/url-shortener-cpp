//
// Created by alxdrzd on 2/8/26.
//


#pragma once

#include <string>
#include <memory>
#include <pqxx/pqxx>

#ifndef DATABASE_H
#define DATABASE_H

class Database {
public:
    explicit Database(const std::string& connection_string);
    ~Database() = default;
    [[nodiscard]] pqxx::connection& get_connection() const;

private:
    std::unique_ptr<pqxx::connection> m_connection;
};

#endif //DATABASE_H
