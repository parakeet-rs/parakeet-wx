#pragma once

#include "um-crypto/common.h"
#include "um-crypto/kugou/KGMCipher.h"

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <shared_mutex>

namespace umc::kugou {

class KGMMaskGenerator {
 public:
  typedef std::shared_ptr<KGMMaskGenerator> Ptr;
  KGMMaskGenerator() {}
  virtual ~KGMMaskGenerator() {}
  void SetTable(const KugouTable& t1,
                const KugouTable& t2,
                const KugouTable& v2);
  void FetchMask(u8* out, usize len, usize offset);
  void FreeMaskMemory();
  void ResetMask();
  static Ptr GetInstance() { return instance_; }

 private:
  static Ptr instance_;

  KugouTable t1_;
  KugouTable t2_;
  KugouTable v2_;

  Vec<u8> v2_mask_;
  std::atomic<usize> v2_mask_len_ = 0;
  std::shared_mutex v2_mask_mutex_;

 private:
  void GenerateMaskUpTo(usize availalbe_offset);
};

}  // namespace umc::kugou
