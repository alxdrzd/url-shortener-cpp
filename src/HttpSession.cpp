#include "HttpSession.h"
#include <iostream>
#include <boost/json.hpp>

namespace json = boost::json;

HttpSession::HttpSession(tcp::socket socket, UrlRepository &repo)
    :   m_socket(std::move(socket)),
        m_repo(repo) {}

void HttpSession::start() {
    do_read();
}

void HttpSession::do_read() {
    auto self = shared_from_this();

    http::async_read(m_socket, m_buffer, m_req,
        [this, self](beast::error_code ec, std::size_t bytes_transferred) {
            if (ec == http::error::end_of_stream) {
                m_socket.shutdown(tcp::socket::shutdown_send, ec);
                return;
            }

            if (ec) {
                return;
            }

             handle_request();
        }
    );
}

void HttpSession::handle_request() {
    http::response<http::string_body> res;
    res.version(m_req.version());
    res.keep_alive(false);

    if (m_req.method() == http::verb::get) {
        std::string target = std::string(m_req.target());

        if (target.length() > 1 && target[0] == '/') {
            std::string short_key = target.substr(1);
            std::optional<std::string> original_url = m_repo.get_original_url(short_key);

            if (original_url) {
                res.result(http::status::found);
                res.set(http::field::location, *original_url);
                res.body() = "Redirecting...";
            } else {
                res.result(http::status::not_found);
                res.body() = "URL was not found in the database";
            }
        } else {
            res.result(http::status::ok);
            res.body() = "Welcome to C++ URL shortener!";
        }
    } else if (m_req.method() == http::verb::post) {
        try {
            json::value request_data = json::parse(m_req.body());
            std::string long_url = request_data.at("original_url").as_string().c_str();
            int user_id = request_data.at("user_id").as_int64();
            std::string short_key = m_repo.save_url(long_url, user_id);
            std::string full_short_url = "https://localhost:8080/" + short_key;

            json::object response_data;
            response_data["short_url"] = full_short_url;
            response_data["short_key"] = short_key;
            response_data["user_id"] = user_id;
            response_data["status"] = "success";

            res.result(http::status::created);
            res.set(http::field::content_type, "application/json");
            res.body() = json::serialize(response_data);
        } catch (const std::exception& e) {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");

            json::object error_data;
            error_data["error"] = e.what();
            error_data["message"] = "JSON must contain 'original_url' (string) and 'user_id' (int)";

            res.body() = json::serialize(error_data);
        }
    }
}





