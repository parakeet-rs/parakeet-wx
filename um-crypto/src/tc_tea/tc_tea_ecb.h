#include <cstdint>
#include "../util/endian.h"

namespace umc::tc_tea {

void ecb_decrypt(void* block, uint32_t* k);

}  // namespace umc::tc_tea
