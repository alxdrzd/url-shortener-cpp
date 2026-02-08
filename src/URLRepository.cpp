//
// Created by alxdrzd on 2/8/26.
//
#include "URLRepository.h"

#include "URLEncoder.h"

URLRepository::URLRepository(Database &db) : m_db(db) { }

std::string URLRepository::save_url(const std::string &original_url, int user_id) {
    pqxx::work txn(m_db.get_connection());

    pqxx::result res = txn.exec_params(
        "INSERT INTO urls (original_url, user_id, short_key) VALUES ($1, $2, '') RETURNING id",
        original_url, user_id
    );

    uint64_t id = res[0][0].as<long long>();

    std::string short_key = URLEncoder::encode(id);

    txn.exec_params(
        "UPDATE urls SET short_key = $1 WHERE id = $2",
        short_key, id
    );

    txn.commit();

    return short_key;
}

std::optional<std::string> URLRepository::get_original_url(const std::string &short_key) {
    pqxx::nontransaction ntxn(m_db.get_connection());

    pqxx::result res = ntxn.exec_params(
        "SELECT original_url FROM urls WHERE short_key = $1",
        short_key
    );

    if (res.empty()) {
        return std::nullopt;
    }

    return res[0][0].as<std::string>();

}
