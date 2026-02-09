cmake_minimum_required(VERSION 3.30)
project(url_shortener_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PQXX REQUIRED libpqxx)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PQXX_INCLUDE_DIRS})

include_directories(include)

add_executable(url_shortener_cpp src/main.cpp
        include/Database.h
        src/Database.cpp
        src/UrlEncoder.cpp
        include/UrlEncoder.h
        include/UrlRepository.h
        src/UrlRepository.cpp
        include/HttpServer.h
        src/HttpServer.cpp
        include/HttpSession.h
        src/HttpSession.cpp)

target_link_libraries(url_shortener_cpp
        ${Boost_LIBRARIES}
        ${PQXX_LIBRARIES}
)