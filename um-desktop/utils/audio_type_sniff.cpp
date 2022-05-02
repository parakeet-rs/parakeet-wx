#include "audio_type_sniff.h"
#include "audio_sniff/mediainfo.h"
#include "audio_sniff/simple.h"

#include <boost/nowide/convert.hpp>

#include <wx/window.h>

namespace nowide = boost::nowide;
using namespace umd::utils;

std::string SniffAudioType(uint8_t* buf, size_t len) {
#if BUILD_WITH_MEDIAINFO
  return audio_sniff_mediainfo(buf, len);
#else
  return audio_sniff_simple(buf, len);
#endif
}
