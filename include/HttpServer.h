//
// Created by alxdrzd on 2/9/26.
//

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "UrlRepository.h"

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

class HttpServer {
public:
    HttpServer(const std::string &address, unsigned short port, UrlRepository &repo, int threads,
               const std::string &static_root_path);

    void run();

private:
    net::io_context m_ioc;
    tcp::acceptor m_acceptor;
    UrlRepository &m_repo;
    int m_threads_count;

    void do_accept();

    std::string m_static_root_path;
};

#endif // HTTPSERVER_H
