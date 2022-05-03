#include "um-crypto/kugou/KGMMaskGenerator.h"

#include <cstring>

namespace umc::kugou {

KGMMaskGenerator::Ptr KGMMaskGenerator::instance_ =
    std::make_shared<KGMMaskGenerator>();

void KGMMaskGenerator::FreeMaskMemory() {
  std::unique_lock guard(v2_mask_mutex_);

  v2_mask_len_.store(0);
  v2_mask_.resize(0);
  v2_mask_.shrink_to_fit();
}

void KGMMaskGenerator::ResetMask() {
  std::unique_lock guard(v2_mask_mutex_);

  v2_mask_len_.store(0);
  v2_mask_.resize(0);
}

void KGMMaskGenerator::SetTable(const KugouTable& t1,
                                const KugouTable& t2,
                                const KugouTable& v2) {
  std::unique_lock guard(v2_mask_mutex_);
  t1_ = t1;
  t2_ = t2;
  v2_ = v2;
  v2_mask_len_.store(0);
  v2_mask_.resize(0);
}

void KGMMaskGenerator::GenerateMaskUpTo(usize target_len) {
  std::unique_lock guard(v2_mask_mutex_);

  usize current_len = v2_mask_len_.load();
  if (target_len <= current_len) {
    return;
  }
  target_len = std::max(kTableSize, target_len);
  v2_mask_.resize(target_len);

  u8* v2_mask = v2_mask_.data();
  const u8* t1 = t1_.data();
  const u8* t2 = t2_.data();
  const u8* v2 = v2_.data();

  for (usize i = current_len; i < kTableSize; i++) {
    v2_mask[i] = v2[i];
  }

  for (usize i = std::max(current_len, kTableSize); i < target_len; i++) {
    ////////////////////////////////////////////////////////////////////////////
    // Algorithm that calculates v2_mask based on previous v2_mask value and
    //   given table t1, t2 & v2.
    //
    // Note: index provided to access t1, t2, v2 must MOD kTableSize.
    ////////////////////////////////////////////////////////////////////////////
    // Base rules:
    // v2_mask[i] => v1_mask[i >> 4] ^ v2[i]
    // v1_mask[i] => t1[i] ^ t2[(i >> 4)] ^ v1_mask[i >> 8]
    // v1_mask[0] => 0
    ////////////////////////////////////////////////////////////////////////////
    // subst: i as i >> 4
    // v1_mask[i >> 4] =>
    //     t1[i >> 4]
    //   ^ t2[i >> 8]
    //   ^ v1_mask[i >> 0x0c]
    //
    // v2_mask[i] =>
    //     t1[i >> 4]
    //   ^ t2[i >> 8]
    //   ^ v1_mask[i >> 0x0c]
    //   ^ v2[i];
    ////////////////////////////////////////////////////////////////////////////
    // Solve: v1_mask[i >> 0x0c]
    // v1_mask[(i >> 8) >> 4] => v2_mask[i >> 8] ^ v2[i >> 8]
    // v1_mask[i >> 0x0c] => v2_mask[i >> 8] ^ v2[i >> 8];
    ////////////////////////////////////////////////////////////////////////////
    // subst all back:
    // v2_mask[i] =>
    //     t1[(i >> 4)]
    //   ^ t2[(i >> 8)]
    //   ^ v2_mask[i >> 8]
    //   ^ v2[i >> 8]
    //   ^ v2[i];
    ////////////////////////////////////////////////////////////////////////////

#define T1(X) (t1[(X) % kTableSize])
#define T2(X) (t2[(X) % kTableSize])
#define V2(X) (v2[(X) % kTableSize])
    v2_mask[i] = v2_mask[i >> 8] ^ T1(i >> 4) ^ T2(i >> 8) ^ V2(i) ^ V2(i >> 8);
#undef T1
#undef T2
#undef V2
  }

  v2_mask_len_.store(target_len);
}

void KGMMaskGenerator::FetchMask(u8* out, usize len, usize offset) {
  const usize min_len_required = offset + len;

  while (true) {
    {
      std::shared_lock guard(v2_mask_mutex_);
      if (min_len_required <= v2_mask_len_.load()) {
        memcpy(out, &v2_mask_[offset], len);
        return;
      }
    }

    GenerateMaskUpTo(min_len_required);
  }
}

}  // namespace umc::kugou
