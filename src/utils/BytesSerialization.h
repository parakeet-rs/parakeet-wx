#pragma once

#include <cstdint>

#include <span>
#include <string>
#include <vector>

namespace parakeet_wx::utils {

std::string SerializeBytes(std::span<const uint8_t> data);
std::vector<uint8_t> DeserializeBytes(const std::string data);

}  // namespace parakeet_wx::utils
