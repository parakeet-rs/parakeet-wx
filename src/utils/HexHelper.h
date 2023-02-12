#pragma once

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

namespace parakeet_wx::utils
{

inline bool IsHexChar(const char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

inline char ToHexChar(uint8_t byte)
{
    static const char table[] = "0123456789ABCDEF";
    return table[byte & 0x0F];
}

inline uint8_t HexLookup(const char c)
{
    if (c >= '0' && c <= '9')
        return static_cast<uint8_t>(c - '0');
    if (c >= 'A' && c <= 'F')
        return static_cast<uint8_t>(c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
        return static_cast<uint8_t>(c - 'a' + 10);

    // we know this is wrong, but hey, we'll accept it.
    return 0;
}

inline std::string UnescapeCharSequence(const std::string &s)
{
    std::string result;
    result.reserve(s.size());
    bool open_escape = false;
    for (const auto c : s)
    {
        if (open_escape)
        {
            switch (c)
            {
            case 'n':
                result.push_back('\n');
                break;
            case 'r':
                result.push_back('\r');
                break;
            case 't':
                result.push_back('\t');
                break;
            case 'f':
                result.push_back('\f');
                break;
            case 'v':
                result.push_back('\v');
                break;

            default:
                result.push_back(c);
                break;
            }
            open_escape = false;
        }
        else if (c == '\\')
        {
            open_escape = true;
        }
        else
        {
            result.push_back(c);
        }
    }

    return result;
}

inline std::vector<uint8_t> UnHexWithTolerance(const char *str, size_t str_len)
{
    if (str_len == 0)
    {
        return {};
    }

    std::vector<uint8_t> result;
    result.reserve(str_len / 2);
    uint8_t hex_char{0};
    bool is_second_char{false};
    std::for_each_n(str, str_len, [&](const auto &chr) {
        if (is_second_char)
        {
            if (IsHexChar(chr))
            {
                result.push_back((hex_char << 4) | HexLookup(chr));
            }
            is_second_char = false;
        }
        else if (IsHexChar(chr))
        {
            hex_char = HexLookup(chr);
            is_second_char = true;
        }
    });

    return result;
}

inline std::string HexWithSpaces(const uint8_t *data, size_t data_len)
{
    if (data_len == 0)
    {
        return {};
    }

    std::string result{};
    result.reserve(data_len * 3);

    std::for_each_n(data, data_len, [&](const auto &byte) {
        result.push_back(ToHexChar(byte >> 4));
        result.push_back(ToHexChar(byte & 0x0F));
        result.push_back(' ');
    });

    result.pop_back();
    return result;
}

} // namespace parakeet_wx::utils
