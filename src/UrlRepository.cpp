//
// Created by alxdrzd on 2/8/26.
//
#include "UrlRepository.h"

#include "UrlEncoder.h"

UrlRepository::UrlRepository(Database &db) : m_db(db) {
}

std::string UrlRepository::save_url(const std::string &original_url) {
    pqxx::work txn(m_db.get_connection());

    pqxx::result res_select = txn.exec_params(
        "SELECT short_key FROM urls WHERE original_url = $1",
        original_url);

    if (!res_select.empty()) {
        auto existing_key = res_select[0][0].as<std::string>();
        txn.commit();
        return existing_key;
    }

    pqxx::result res = txn.exec_params(
        "INSERT INTO urls (original_url, short_key) VALUES ($1, '') RETURNING id",
        original_url);

    uint64_t id = res[0][0].as<long long>();

    std::string short_key = UrlEncoder::encode(id);

    txn.exec_params(
        "UPDATE urls SET short_key = $1 WHERE id = $2",
        short_key, id);

    txn.commit();

    return short_key;
}

std::optional<std::string> UrlRepository::get_original_url(const std::string &short_key) {
    pqxx::nontransaction ntxn(m_db.get_connection());

    pqxx::result res = ntxn.exec_params(
        "SELECT original_url FROM urls WHERE short_key = $1",
        short_key);

    if (res.empty()) {
        return std::nullopt;
    }

    return res[0][0].as<std::string>();
}
