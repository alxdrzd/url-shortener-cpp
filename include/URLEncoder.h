//
// Created by alxdrzd on 2/8/26.
//

#pragma once

#include <string>
#include <cstdint>

#ifndef URLENCODER_H
#define URLENCODER_H

class URLEncoder {
public:
    static std::string encode(std::uint64_t id);
};

#endif //URLENCODER_H
