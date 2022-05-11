#pragma once

#include <algorithm>
#include <memory>

#include "um-crypto/types.h"

namespace umc::decryption {

class DecryptionStream {
 public:
  /**
   * @brief Reset and seek to begin of file.
   *
   */
  virtual void Reset() {
    buf_in_.resize(0);
    buf_out_.resize(0);
    offset_ = 0;
  }

  /**
   * @brief Write encrypted data stream to the file loader.
   *
   * @param in
   * @param len
   */
  virtual bool Write(const u8* in, usize len) = 0;
  /**
   * @brief Notify stream transformer that we have reached end of file.
   */
  virtual bool End() = 0;

  virtual const Str GetName() const = 0;

  /**
   * @brief Return true if the decryptor is in an error state.
   *
   * @return true
   * @return false
   */
  virtual bool IsBad() const { return error_; }

  inline usize GetOutputSize() { return buf_out_.size(); }
  inline usize Peek(u8* out, usize len) {
    len = std::min(len, buf_out_.size());
    std::copy_n(buf_out_.begin(), len, out);
    return len;
  }
  inline usize Read(u8* out, usize len) {
    usize read_len = Peek(out, len);
    buf_out_.erase(buf_out_.begin(), buf_out_.begin() + read_len);
    return read_len;
  }
  inline void ReadAll(Vec<u8>& out) { out = std::move(buf_out_); }

 protected:
  usize offset_ = 0;
  bool error_ = false;

  Vec<u8> buf_in_;
  Vec<u8> buf_out_;

  /**
   * @brief Downgrade state to error and no upgrades.
   *
   * @param good The current operation status.
   */
  inline void UpdateErrorBasedOnGood(bool good) { error_ = error_ || !good; }

  /**
   * @brief Encrypted file - header/offset process helper.
   *        Once header is processed, you should reset `buf_in_`.
   *
   * @param p Pointer to input buffer.
   * @param len input buf size.
   * @param offset bytes to read until reaching this offset.
   * @return true `buf_in_` now contains enough header data.
   * @return false Nothing to do.
   */
  inline bool ReadUntilOffset(const u8*& p, usize& len, usize target_offset) {
    if (offset_ < target_offset) {
      auto read_size = std::min(target_offset - offset_, len);
      if (read_size == 0) return false;

      buf_in_.insert(buf_in_.end(), p, p + read_size);

      offset_ += read_size;
      p += read_size;
      len -= read_size;
    }

    return offset_ == target_offset;
  }

  /**
   * @brief Keep reading data from p to buf_in_, until it reaches block_size.
   *        Once block is processed, you should reset `buf_in_`.
   *
   * @param p
   * @param len
   * @param block_size
   * @return true
   * @return false
   */
  inline bool ReadBlock(const u8*& p, usize& len, usize block_size) {
    if (buf_in_.size() < block_size) {
      auto read_size = std::min(block_size - buf_in_.size(), len);
      if (read_size == 0) return false;

      buf_in_.insert(buf_in_.end(), p, p + read_size);

      p += read_size;
      len -= read_size;
    }

    return buf_in_.size() == block_size;
  }
};

}  // namespace umc::decryption
