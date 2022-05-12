#pragma once

#include "../DecryptionStream.h"

namespace umc::decryption::kugou {

constexpr usize kKugouInternalTableSize = 17 * 16;
constexpr usize kKugouVprKeySize = 17;

typedef Arr<u8, kKugouInternalTableSize> KugouInternalTable;
typedef Arr<u8, kKugouVprKeySize> KugouVPRKey;

class KugouFileLoader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "Kugou(KGM/VPR)"; };

  /**
   * @brief Create KugouFileLoader for KGM.
   */
  static std::unique_ptr<KugouFileLoader> Create(const KugouInternalTable& t1,
                                                 const KugouInternalTable& t2,
                                                 const KugouInternalTable& v2,
                                                 bool use_cache);
  /**
   * @brief Create KugouFileLoader for VPR.
   */
  static std::unique_ptr<KugouFileLoader> Create(const KugouInternalTable& t1,
                                                 const KugouInternalTable& t2,
                                                 const KugouInternalTable& v2,
                                                 const KugouVPRKey& vpr_key,
                                                 bool use_cache);
};

}  // namespace umc::decryption::kugou
