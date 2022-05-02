#include "audio_type_sniff.h"

#include <boost/nowide/convert.hpp>

#include <wx/window.h>

namespace nowide = boost::nowide;

namespace umd::utils {

std::string SniffAudioType(const uint8_t* buf, size_t len) {
#if BUILD_WITH_MEDIAINFO
  return AudioSniffMediainfo(buf, len);
#else
  return AudioSniffSimple(buf, len);
#endif
}

}  // namespace umd::utils
