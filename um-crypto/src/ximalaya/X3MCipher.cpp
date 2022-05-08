#include "um-crypto/ximalaya/X3MCipher.h"

#include <algorithm>

namespace umc::ximalaya {

XimalayaHeaderScrambleTable X3MCipher::scramble_table_ = {};
XimalayaHeaderContentKey X3MCipher::content_key_ = {};

X3MCipher::X3MCipher(const XimalayaAndroidFileHeader& header,
                     const XimalayaHeaderContentKey& content_key,
                     const XimalayaHeaderScrambleTable& scramble_table)
    : AXimalayaAndroidHeaderCipher(header, content_key, scramble_table) {}

X3MCipher::X3MCipher(const XimalayaAndroidFileHeader& header)
    : X3MCipher(header, content_key_, scramble_table_) {}

void X3MCipher::SetScrambleTable(const XimalayaHeaderScrambleTable& table) {
  scramble_table_ = table;
}

void X3MCipher::SetContentKey(const XimalayaHeaderContentKey& key) {
  content_key_ = key;
}

}  // namespace umc::ximalaya
