#include "um-crypto/tencent/SimpleEKeyDerive.h"
#include <cmath>

namespace umc::tencent {

void SimpleEKeyDerive::MakeSimpleKey(Vec<u8>& out) const {
  double seed = static_cast<double>(this->seed);
  for (auto& byte : out) {
    byte = static_cast<uint8_t>(fabs(tan(seed)) * 100.0);
    seed += 0.1;
  }
}

}  // namespace umc::tencent
