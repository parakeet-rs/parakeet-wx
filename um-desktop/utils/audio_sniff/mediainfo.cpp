#include "mediainfo.h"

#if BUILD_WITH_MEDIAINFO
#include <MediaInfo/MediaInfo.h>
#include <boost/nowide/convert.hpp>

using namespace MediaInfoLib;
using namespace umd::utils;

std::string audio_sniff_mediainfo(const uint8_t* buf, size_t len) {
  MediaInfo mi;
  mi.Open_Buffer_Init();
  mi.Open_Buffer_Continue(buf, len);
  mi.Open_Buffer_Finalize();

  String format = mi.Get(Stream_General, 0, _T("Format"), Info_Text, Info_Name);

  auto fmt_str = boost::nowide::narrow(format);
  std::transform(fmt_str.begin(), fmt_str.end(), fmt_str.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return fmt_str;
}
#else
std::string audio_sniff_mediainfo(uint8_t* buf, size_t len) {
  return "unknown";
}
#endif
