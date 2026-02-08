//
// Created by alxdrzd on 2/8/26.
//

#pragma once

#include "Database.h"
#include <string>
#include <optional>

#ifndef URLREPOSITORY_H
#define URLREPOSITORY_H

class URLRepository {
public:
    explicit URLRepository(Database& db);
    std::string save_url(const std::string& original_url, int user_id);
    std::optional<std::string> get_original_url(const std::string& short_key);
private:
    Database& m_db;
};

#endif //URLREPOSITORY_H
