#include "HttpSession.h"
#include <iostream>
#include <fstream>
#include <boost/json.hpp>

namespace json = boost::json;

HttpSession::HttpSession(tcp::socket socket, UrlRepository &repo, const std::string &static_root_path)
    : m_socket(std::move(socket)),
      m_repo(repo),
      m_static_root_path(static_root_path) {
}

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
                     });
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
            std::string file_path = m_static_root_path + "/index.html";
            std::ifstream file(file_path);

            if (file) {
                std::stringstream buffer;

                buffer << file.rdbuf();

                res.result(http::status::ok);
                res.set(http::field::content_type, "text/html");
                res.body() = buffer.str();
            } else {
                res.result(http::status::not_found);
                res.body() = "Not Found: index.html is missing on the server.";
            }
        }
    } else if (m_req.method() == http::verb::post) {
        try {
            json::value request_data = json::parse(m_req.body());
            std::string long_url = request_data.at("original_url").as_string().c_str();

            const std::regex url_pattern(
                R"(^https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*)$)");

            if (!std::regex_match(long_url, url_pattern)) {
                throw std::runtime_error(
                    "Invalid URL format. It must start with http:// or https:// and have a domain.");
            }

            std::string short_key = m_repo.save_url(long_url);
            std::string full_short_url = "http://localhost:8080/" + short_key;

            json::object response_data;
            response_data["short_url"] = full_short_url;
            response_data["short_key"] = short_key;
            response_data["status"] = "success";

            res.result(http::status::created);
            res.set(http::field::content_type, "application/json");
            res.body() = json::serialize(response_data);
        } catch (const std::exception &e) {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");

            json::object error_data;
            error_data["error"] = "Validation Error";
            error_data["message"] = e.what();

            res.body() = json::serialize(error_data);
        }
    }
    res.prepare_payload();
    do_write(std::move(res));
}

void HttpSession::do_write(http::response<http::string_body> res) {
    auto self = shared_from_this();

    auto sp_res = std::make_shared<http::response<http::string_body> >(std::move(res));

    http::async_write(m_socket, *sp_res,
                      [this, self, sp_res](beast::error_code ec, std::size_t) {
                          (void) m_socket.shutdown(tcp::socket::shutdown_send, ec);
                      });
}
