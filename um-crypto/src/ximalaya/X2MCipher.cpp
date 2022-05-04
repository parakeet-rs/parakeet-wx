#include "um-crypto/ximalaya/X2MCipher.h"

#include <algorithm>

namespace umc::ximalaya {

const u8 kX2MKey_xmly[] = {'x', 'm', 'l', 'y'};

X2MCipher::X2MCipher(const X2MHeader& header) : IStreamCipher() {
  header_ = header;
}

constexpr double kX2MMultiplier = 3.837465;
constexpr double kX2MInitialDelta = 0.615243;

static_assert(kX2MInitialDelta > 0.0 && kX2MInitialDelta < 1.0,
              "X2M: Invalid initial delta.");
static_assert(kX2MMultiplier > 3.56994 && kX2MMultiplier < 4.0,
              "X2M: Invalid multiplier.");

const X2MScrambleTable X2MCipher::scramble_table_ = []() {
  X2MScrambleTable out_swap_indexes = {};
  std::array<double, kX2MKeySize> table;

  // Populate search table
  table[0] = kX2MInitialDelta;
  double delta = kX2MInitialDelta;
  for (int i = 1; i < kX2MKeySize; i++) {
    delta = delta * kX2MMultiplier * (1.0 - delta);
    table[i] = delta;
  }

  // Create a copy of the unsorted table, and sort it:
  std::array<double, kX2MKeySize> sorted(table);
  std::sort(sorted.begin(), sorted.end(), std::less<double>{});

  std::array<bool, kX2MKeySize> swapped;
  for (int i = 0; i < kX2MKeySize; i++) {
    // search only once
    auto p = std::find(sorted.begin(), sorted.end(), table[i]);
    out_swap_indexes[i] = p - sorted.begin();
    *p = 0;
  }

  return std::move(out_swap_indexes);
}();

bool X2MCipher::Decrypt(u8* p_out,
                        usize& out_len,
                        const u8* p_in,
                        usize in_len) {
  if (out_len < in_len) {
    out_len = in_len;
    return false;
  }
  out_len = in_len;

  usize offset = offset_;
  if (offset < kX2MEncryptedHeaderSize) {
    usize len = std::min(in_len, kX2MEncryptedHeaderSize - offset);
    for (usize i = 0; i < len; i++) {
      const auto idx = static_cast<usize>(scramble_table_[offset]);
      *p_out++ = header_[idx] ^ kX2MKey_xmly[offset & 3];
      offset++;
    }
    p_in += len;
    in_len -= len;
    offset_ = offset;
  }

  if (in_len > 0) {
    offset_ += in_len;
    std::copy(p_in, p_in + in_len, p_out);
  }
  return true;
}

}  // namespace umc::ximalaya
