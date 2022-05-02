#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace umd::utils {

std::string SniffAudioType(const uint8_t* buf, size_t len);
std::string AudioSniffMediainfo(const uint8_t* buf, size_t len);
std::string AudioSniffSimple(const uint8_t* buf, size_t len);

}  // namespace umd::utils
