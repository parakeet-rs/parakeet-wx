#include "um-crypto/ximalaya/X2MCipher.h"

#include <algorithm>

namespace umc::ximalaya {

constexpr double kX2MMultiplier = 3.837465;
constexpr double kX2MInitialDelta = 0.615243;

static_assert(kX2MInitialDelta > 0.0 && kX2MInitialDelta < 1.0,
              "X2M: Invalid initial delta.");
static_assert(kX2MMultiplier > 3.56994 && kX2MMultiplier < 4.0,
              "X2M: Invalid multiplier.");

XimalayaHeaderScrambleTable X2MCipher::scramble_table_ = []() {
  XimalayaHeaderScrambleTable scramble_table = {};
  Arr<double, kXimalayaEncryptedHeaderSize> table;

  // Populate search table
  table[0] = kX2MInitialDelta;
  double delta = kX2MInitialDelta;
  for (int i = 1; i < kXimalayaEncryptedHeaderSize; i++) {
    delta = delta * kX2MMultiplier * (1.0 - delta);
    table[i] = delta;
  }

  // Create a copy of the unsorted table, and sort it:
  std::array<double, kXimalayaEncryptedHeaderSize> sorted(table);
  std::sort(sorted.begin(), sorted.end(), std::less<double>{});

  std::array<bool, kXimalayaEncryptedHeaderSize> swapped;
  for (int i = 0; i < kXimalayaEncryptedHeaderSize; i++) {
    // search only once
    auto p = std::find(sorted.begin(), sorted.end(), table[i]);
    scramble_table[i] = p - sorted.begin();
    *p = 0;
  }

  return scramble_table;
}();

XimalayaHeaderContentKey X2MCipher::content_key_ = {};

X2MCipher::X2MCipher(const XimalayaAndroidFileHeader& header,
                     const XimalayaHeaderContentKey& content_key,
                     const XimalayaHeaderScrambleTable& scramble_table)
    : AXimalayaAndroidHeaderCipher(header, content_key, scramble_table) {}

X2MCipher::X2MCipher(const XimalayaAndroidFileHeader& header)
    : X2MCipher(header, content_key_, scramble_table_) {}

void X2MCipher::SetScrambleTable(const XimalayaHeaderScrambleTable& table) {
  auto p_input = table.data();
  for (auto p = scramble_table_.begin(); p < scramble_table_.end(); p += 4) {
    std::copy_n(p_input, 4, p);
  }
}

void X2MCipher::SetContentKey(const XimalayaHeaderContentKey& key) {
  content_key_ = key;
}

}  // namespace umc::ximalaya
