#include "um-crypto/ximalaya/X2MCipher.h"

#include <algorithm>

namespace umc::ximalaya {

constexpr double kX2MMultiplier = 3.837465;
constexpr double kX2MInitialDelta = 0.615243;

static_assert(kX2MInitialDelta > 0.0 && kX2MInitialDelta < 1.0,
              "X2M: Invalid initial delta.");
static_assert(kX2MMultiplier > 3.56994 && kX2MMultiplier < 4.0,
              "X2M: Invalid multiplier.");

const XimalayaHeaderScrambleTable kX2MHeaderScrambleTable = []() {
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

const XimalayaHeaderContentKey kX2MHeaderContentKey = []() {
  XimalayaHeaderContentKey key = {};
  for (int i = 0; i < kXimalayaHeaderContentKeySize; i += 4) {
    key[i + 0] = 'x';
    key[i + 1] = 'm';
    key[i + 2] = 'l';
    key[i + 3] = 'y';
  }

  return key;
}();

X2MCipher::X2MCipher(const XimalayaAndroidFileHeader& header)
    : AXimalayaAndroidHeaderCipher(header,
                                   kX2MHeaderContentKey,
                                   kX2MHeaderScrambleTable) {}

}  // namespace umc::ximalaya
