#pragma once

#include "um-crypto/decryption/DecryptionStream.h"

#include <map>

namespace umc::decryption::kugou {

typedef Vec<u8> KugouSingleSlotKey;
typedef std::map<uint32_t, KugouSingleSlotKey> KugouSlotKeys;
typedef Vec<u8> KugouV4SlotKeyExpansionTable;
typedef Vec<u8> KugouV4FileKeyExpansionTable;

class KugouFileLoader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "Kugou"; };

  /**
   * @brief Create KugouFileLoader for KGM / VPR.
   */
  static std::unique_ptr<KugouFileLoader> Create(
      const KugouSlotKeys& slot_keys,
      const KugouV4SlotKeyExpansionTable& v4_slot_key_expansion_table,
      const KugouV4FileKeyExpansionTable& v4_file_key_expansion_table);
};

}  // namespace umc::decryption::kugou
