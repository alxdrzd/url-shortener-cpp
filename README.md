# High-Performance URL Shortener (C++ Microservice)

A high-performance, asynchronous microservice for shortening URLs, built with **modern C++** and **PostgreSQL**. The project demonstrates a clean layered architecture, concurrent request handling using **Boost.Asio**, and full containerization with **Docker**.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Boost](https://img.shields.io/badge/Boost.Asio-1.80+-green.svg)
![PostgreSQL](https://img.shields.io/badge/PostgreSQL-16+-blue.svg)
![Docker](https://img.shields.io/badge/Docker-Compose-2496ED.svg)

## 🚀 Key Features

*   **Asynchronous I/O:** Built on top of `Boost.Asio` and `Boost.Beast` for non-blocking HTTP request handling.
*   **Multithreading:** Utilizes a thread pool to handle concurrent connections efficiently.
*   **Layered Architecture:** Clear separation of concerns:
    *   *Network Layer* (HTTP handling)
    *   *Service Layer* (Business logic & Base62 encoding)
    *   *Data Layer* (Repository pattern with `libpqxx`)
*   **Database Integration:** PostgreSQL with automated schema migration.
*   **Idempotency:** Checks for existing URLs before creating duplicates to save DB space.
*   **Frontend:** A simple HTML/JS web interface for testing.
*   **Containerization:** Fully Dockerized with multi-stage builds (reducing image size).

## 🛠 Tech Stack

*   **Language:** C++17
*   **Networking:** Boost.Asio, Boost.Beast
*   **JSON Parsing:** Boost.JSON
*   **Database:** PostgreSQL, libpqxx (C++ client)
*   **Build System:** CMake
*   **Deployment:** Docker & Docker Compose

## 📂 Project Structure

```text
.
├── src/                # Source files (.cpp)
│   ├── main.cpp        # Entry point & connection logic
│   ├── HttpServer.cpp  # Connection acceptor & thread pool
│   ├── HttpSession.cpp # Request handling & business logic
│   ├── UrlRepository.cpp # Database interactions
│   └── ...
├── include/            # Header files (.h)
├── sql/                # SQL migration scripts
│   └── schema.sql      # Database schema
├── static/             # Frontend files
│   └── index.html      # Web interface
├── Dockerfile          # Docker build
├── docker-compose.yml  # Docker .yml file 
└── CMakeLists.txt      # Build configuration
