//
// Created by alxdrzd on 2/8/26.
//

#include "UrlEncoder.h"
#include <algorithm>

const std::string ALPHABET = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::string UrlEncoder::encode(std::uint64_t id) {
    if (id == 0) {
        return std::string(1, ALPHABET[0]);
    }

    std::string short_url;

    while (id > 0) {
        int remainder = id % 62;
        short_url += ALPHABET[remainder];
        id /= 62;
    }

    std::reverse(short_url.begin(), short_url.end());

    return short_url;
}
