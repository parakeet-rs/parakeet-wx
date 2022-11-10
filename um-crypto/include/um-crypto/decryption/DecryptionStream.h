#pragma once

#include <algorithm>
#include <memory>

#include "um-crypto/types.h"

namespace umc::decryption {

constexpr std::size_t kDetectionBufferLen = 4096;
typedef std::array<u8, kDetectionBufferLen> DetectionBuffer;

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
   * @brief Initialise decryptor with data found in file footer.
   *
   * @param buf
   * @return std::size_t Bytes to reserve and don't seed to this decryptor.
   */
  virtual std::size_t InitWithFileFooter(const DetectionBuffer& buf) {
    return 0;
  }

  /**
   * @brief Write encrypted data stream to the file loader.
   *
   * @param in
   * @param len
   */
  virtual bool Write(const u8* in, std::size_t len) = 0;
  /**
   * @brief Notify stream transformer that we have reached end of file.
   */
  virtual bool End() = 0;

  virtual const std::string GetName() const = 0;

  /**
   * @brief Return true if the decryptor is in an error state.
   *
   * @return true
   * @return false
   */
  virtual bool InErrorState() const { return !error_.empty(); }

  /**
   * @brief Get the Error Message object
   *
   * @return const std::string&
   */
  virtual const std::string& GetErrorMessage() const { return error_; }

  inline std::size_t GetOutputSize() { return buf_out_.size(); }
  inline std::size_t Peek(u8* out, std::size_t len) {
    len = std::min(len, buf_out_.size());
    std::copy_n(buf_out_.begin(), len, out);
    return len;
  }
  inline std::size_t Read(u8* out, std::size_t len) {
    std::size_t read_len = Peek(out, len);
    buf_out_.erase(buf_out_.begin(), buf_out_.begin() + read_len);
    return read_len;
  }
  inline void ReadAll(std::vector<u8>& out) { out = std::move(buf_out_); }

 protected:
  std::size_t offset_ = 0;
  std::string error_ = "";

  std::vector<u8> buf_in_;
  std::vector<u8> buf_out_;

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
  inline bool ReadUntilOffset(const u8*& p,
                              std::size_t& len,
                              std::size_t target_offset) {
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
  inline bool ReadBlock(const u8*& p,
                        std::size_t& len,
                        std::size_t block_size) {
    if (buf_in_.size() < block_size) {
      auto read_size = std::min(block_size - buf_in_.size(), len);
      if (read_size == 0) return false;

      buf_in_.insert(buf_in_.end(), p, p + read_size);

      p += read_size;
      len -= read_size;
    }

    return buf_in_.size() == block_size;
  }

  inline u8* ExpandOutputBuffer(std::size_t len) {
    std::size_t pos = buf_out_.size();
    buf_out_.resize(pos + len);
    return &buf_out_[pos];
  }

  inline void ConsumeInput(std::size_t len) {
    buf_in_.erase(buf_in_.begin(), buf_in_.begin() + len);
    offset_ += len;
  }

  inline void ConsumeInput(void* out, std::size_t len) {
    std::copy_n(buf_in_.begin(), len, reinterpret_cast<u8*>(out));
    ConsumeInput(len);
  }
};

}  // namespace umc::decryption
