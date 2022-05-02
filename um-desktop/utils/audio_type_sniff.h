#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace umd {

std::string SniffAudioType(uint8_t* buf, size_t len);

}  // namespace umd
