#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "UrlRepository.h"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

#ifndef HTTPSESSION_H
#define HTTPSESSION_H

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket, UrlRepository& repo, const std::string& static_root_path);
    void start();
private:
    tcp::socket m_socket;
    beast::flat_buffer m_buffer;
    http::request<http::string_body> m_req;
    UrlRepository& m_repo;

    void do_read();

    void handle_request();

    void do_write(http::response<http::string_body> res);

    std::string m_static_root_path;
};

#endif //HTTPSESSION_H
