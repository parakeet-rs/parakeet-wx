#include "BytesSerialization.h"

#include "HexHelper.h"

#include <cppcodec/base64_rfc4648.hpp>

#include <cctype>

#include <algorithm>
#include <cstdint>
#include <string>

namespace parakeet_wx::utils
{

using base64 = cppcodec::base64_rfc4648;

std::string SerializeBytes(const uint8_t *data, size_t len)
{
    if (len == 0)
    {
        return "";
    }

    if (std::all_of(data, data + len, isprint))
    {
        return std::string("raw:") + std::string(data, data + len);
    }

    // prefer hex than base64.
    return std::string("hex:") + HexWithSpaces(data, len);
}

bool str_starts_with(const std::string &to_test, const char *prefix)
{
    return std::equal(prefix, prefix + strlen(prefix), to_test.begin());
}

std::vector<uint8_t> DeserializeBytes(const std::string str)
{
    if (str.empty())
        return {};

    if (str_starts_with(str, "raw:"))
    {
        return std::vector<uint8_t>(str.begin() + 4, str.end());
    }

    if (str_starts_with(str, "hex:"))
    {
        return UnHexWithTolerance(&str.at(4), str.size() - 4);
    }

    return base64::decode(str);
}

} // namespace parakeet_wx::utils
