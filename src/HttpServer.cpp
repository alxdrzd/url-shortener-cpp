//
// Created by alxdrzd on 2/9/26.
//

#include "HttpServer.h"
#include "HttpSession.h"
#include <iostream>

HttpServer::HttpServer(const std::string &address, unsigned short port, UrlRepository &repo, int threads, const std::string& static_root_path)
    :   m_ioc(threads),
        m_acceptor(m_ioc),
        m_repo(repo),
        m_threads_count(threads),
        m_static_root_path(static_root_path)
{
    auto const ip_address = net::ip::make_address(address);

    tcp::endpoint endpoint(ip_address, port);

    m_acceptor.open(endpoint.protocol());

    m_acceptor.set_option(net::socket_base::reuse_address(true));

    m_acceptor.bind(endpoint);

    m_acceptor.listen(net::socket_base::max_listen_connections);

    do_accept();
}

void HttpServer::run() {
    std::cout << "Server starting on port " << m_acceptor.local_endpoint().port()
              << " with " << m_threads_count << " threads..." << std::endl;

    std::vector<std::thread> threads;

    for (int i = 0; i < m_threads_count; ++i) {
        threads.emplace_back([this] {
            m_ioc.run();
        });
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void HttpServer::do_accept() {
    auto socket = std::make_shared<tcp::socket>(m_ioc);

    m_acceptor.async_accept(*socket, [this, socket](beast::error_code ec) {
        if (!ec) {
            std::make_shared<HttpSession>(std::move(*socket), m_repo, m_static_root_path)->start();
            std::cout << "New connection accepted!" << std::endl;
        } else {
            std::cerr << "Accept error: " << ec.message() << std::endl;
        }
        do_accept();
    });
}


