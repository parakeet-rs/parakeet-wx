#include "parakeet-audio/audio_metadata.h"
#include "parakeet-audio/detect_audio_type.h"
#include <parakeet-crypto/IStream.h>
#include <parakeet-crypto/StreamHelper.h>

#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace parakeet_wx::decryption_manager
{

class AudioDetectionDestination final : public parakeet_crypto::IWriteable
{
  public:
    AudioDetectionDestination()
    {
        buffer_.reserve(parakeet_audio::kAudioTypeSniffBufferSize);
    }

    bool Write(const uint8_t *buffer, size_t len)
    {
        buffer_.insert(buffer_.end(), buffer, buffer + len);

        auto buffer_size = buffer_.size();

        auto p_buffer = buffer_.data();
        auto p_buffer_end = p_buffer + buffer_size;

        auto metadata_size = parakeet_audio::GetAudioHeaderMetadataSize(p_buffer, buffer_.size());
        if (metadata_size > 0)
        {
            p_buffer += metadata_size;
            if (p_buffer >= p_buffer_end)
            {
                return true; // continue
            }
            buffer_size -= metadata_size;
        }

        // Continue if we still need to fetch some data.
        return buffer_size < parakeet_audio::kAudioTypeSniffBufferSize;
    }

  private:
    std::vector<uint8_t> buffer_{};

  public:
    std::vector<uint8_t> &GetBuffer()
    {
        return buffer_;
    }

    parakeet_audio::AudioType GetAudioType()
    {
        return parakeet_audio::DetectAudioType(buffer_.data(), buffer_.size());
    }
};

} // namespace parakeet_wx::decryption_manager
