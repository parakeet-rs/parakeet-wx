#pragma once

#include <cstdint>

namespace umc::decryption::kugou {

#pragma pack(push, 4)
struct kgm_file_header {
  // 偏移: 0x00
  uint8_t magic[0x10];  // 固定内容

  // 偏移: 0x10
  uint32_t offset_to_data;  // 到加密数据处的文件偏移
  uint32_t encryption_type;  // 加密类型，可以是 2 / 3 / 4 这三个值之一。
  uint32_t key_slot;  // 密钥槽号

  // 偏移: 0x1c
  uint8_t key_challenge[0x10];  // 文件密钥合法性验证
  uint8_t key[0x10];            // 文件密钥
};
#pragma pack(pop)

}  // namespace umc::decryption::kugou
