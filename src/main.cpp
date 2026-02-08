#include <iostream>
#include <boost/asio.hpp>
#include <pqxx/pqxx>

int main() {

    boost::asio::io_context io;
    std::cout << "Boost Asio works!" << std::endl;

    std::cout << "Libpqxx version: " << PQXX_VERSION_MAJOR << "." << PQXX_VERSION_MINOR << std::endl;

    return 0;
}