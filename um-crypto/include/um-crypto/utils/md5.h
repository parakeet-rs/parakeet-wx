#pragma once

#include <array>
#include <cstdint>
#include <span>

namespace umc::utils {

std::array<uint8_t, 16> md5(const std::span<const uint8_t> data);

}
