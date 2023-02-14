#pragma once

#include <cstdint>

#include <string>
#include <vector>

namespace parakeet_wx::utils
{

std::string SerializeBytes(const uint8_t *data, size_t len);
inline std::string SerializeBytes(const std::vector<uint8_t> &data)
{
    return SerializeBytes(data.data(), data.size());
}

std::vector<uint8_t> DeserializeBytes(const std::string data);

} // namespace parakeet_wx::utils
