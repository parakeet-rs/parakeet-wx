#include "um-crypto/decryption/tencent/QMCv2Loader.h"
#include "um-crypto/utils/StringHelper.h"

namespace umc::decryption::tencent {

namespace detail {

constexpr std::size_t kFirstSegmentSize = 0x80;
constexpr std::size_t kOtherSegmentSize = 0x1400;

enum class State {
  kDecryptFirstSegment = 0,
  kDecryptOtherSegment,
};

class QMCv2LoaderImpl : public QMCv2Loader {
 private:
  std::string name_;
  State state_ = State::kDecryptFirstSegment;
  std::shared_ptr<misc::tencent::QMCFooterParser> parser_;

 public:
  QMCv2LoaderImpl(std::shared_ptr<misc::tencent::QMCFooterParser> parser) : parser_(parser) {
    if (!parser) {
      throw std::invalid_argument("QMCv2LoaderImpl: parser should not be null");
    }
  }

  virtual std::size_t InitWithFileFooter(const DetectionBuffer& buf) {
    if (parser_) {
      auto parsed = parser_->Parse(buf.data(), buf.size());
      if (parsed && parsed->key.size() >= 300) {
        InitWithKey(parsed->key);
        return parsed->eof_bytes_ignore;
      }
    }

    error_ = "QMC footer parser not set";
    return 0;
  }

  bool Write(const u8* in, std::size_t len) override {
    while (len) {
      switch (state_) {
        case State::kDecryptFirstSegment:
          if (ReadBlock(in, len, kFirstSegmentSize)) {
            DecryptFirstSegment();
            state_ = State::kDecryptOtherSegment;
          }
          break;

        case State::kDecryptOtherSegment:
          DecryptOtherSegment(in, len);
          return true;
      }
    }

    return len == 0;
  };

  bool End() override { return !InErrorState(); };

 private:
  std::vector<u8> key_;
  std::vector<u8> S_;
  std::size_t N_;
  double key_hash_;
  std::size_t segment_id_ = 0;

  inline void InitWithKey(const std::vector<u8>& key) {
    key_ = key;
    N_ = key.size();
    S_.resize(N_);
    key_hash_ = CalculateKeyHash();
  }

  inline double CalculateKeyHash() const {
    const auto N = N_;

    u32 hash = 1;
    for (u32 i = 0; i < N_; i++) {
      auto value = i32{key_[i]};

      // ignore if key char is '\x00'
      if (!value) continue;

      const u32 next_hash = hash * value;
      if (next_hash == 0 || next_hash <= hash) break;

      hash = next_hash;
    }

    return static_cast<double>(hash);
  }

  inline u64 GetSegmentKey(u64 segment_id, u64 seed) const {
    // HACK: Workaround incorrect behaviour when divided by 0.
    if (seed == 0) {
      return 0;
    }
    return u64(key_hash_ / double((segment_id + 1) * seed) * 100.0);
  }

  void DecryptFirstSegment() {
    std::size_t N = N_;
    auto p_out = ExpandOutputBuffer(kFirstSegmentSize);
    u8* p_in = buf_in_.data();

    for (std::size_t i = 0; i < kFirstSegmentSize; i++) {
      const u64 seed = u64{key_[i % N]};
      p_out[i] = p_in[i] ^ key_[GetSegmentKey(i, seed) % N];
    }

    ConsumeInput(kFirstSegmentSize);

    ResetOtherSegment(kFirstSegmentSize);
  }

  u32 rc4_i_ = 0;
  u32 rc4_j_ = 0;
  std::size_t segment_bytes_left_ = 0;

  inline u8 GetNextRC4Output() {
    // Set alias
    const auto N = N_;
    auto& S = S_;
    auto& i = rc4_i_;
    auto& j = rc4_j_;

    i = (i + 1) % N;
    j = (S[i] + j) % N;
    std::swap(S[i], S[j]);

    return S[(S[i] + S[j]) % N];
  }

  inline void ResetOtherSegment(std::size_t extra_discard = 0) {
    if (segment_bytes_left_ != 0) return;

    auto& S = S_;
    const auto N = N_;

    // Reset all
    rc4_i_ = rc4_j_ = 0;
    for (u32 i = 0; i < N; i++) {
      S[i] = i & 0xFF;
    }

    u32 j = 0;
    for (u32 i = 0; i < N; i++) {
      j = (S[i] + j + key_[i % N]) % N;
      std::swap(S[i], S[j]);
    }

    size_t seed = key_[segment_id_ & 0x1FF];
    auto discards = extra_discard + (GetSegmentKey(segment_id_, seed) & 0x1FF);
    segment_bytes_left_ = kOtherSegmentSize - extra_discard;

    for (u32 i = 0; i < discards; i++) {
      GetNextRC4Output();
    }

    segment_id_++;
  }

  void DecryptOtherSegment(const u8* in, std::size_t len) {
    auto p_out = ExpandOutputBuffer(len);

    auto& S = S_;
    const auto N = N_;

    while (len > 0) {
      ResetOtherSegment();
      std::size_t processed_len = std::min(segment_bytes_left_, len);
      for (u32 i = 0; i < processed_len; i++) {
        p_out[i] = in[i] ^ GetNextRC4Output();
      }

      in += processed_len;
      p_out += processed_len;

      len -= processed_len;
      segment_bytes_left_ -= processed_len;

      offset_ += processed_len;
    }
  }
};

}  // namespace detail

std::unique_ptr<QMCv2Loader> QMCv2Loader::Create(std::shared_ptr<misc::tencent::QMCFooterParser> parser) {
  return std::make_unique<detail::QMCv2LoaderImpl>(parser);
}

}  // namespace umc::decryption::tencent
