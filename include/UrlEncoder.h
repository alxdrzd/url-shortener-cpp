#pragma once

#include <string>
#include <cstdint>

#ifndef URLENCODER_H
#define URLENCODER_H

class UrlEncoder {
public:
    static std::string encode(std::uint64_t id);
};

#endif // URLENCODER_H
