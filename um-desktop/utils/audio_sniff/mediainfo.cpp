#include "../audio_type_sniff.h"

#if BUILD_WITH_MEDIAINFO
#include <MediaInfo/MediaInfo.h>
#include <boost/nowide/convert.hpp>

using namespace MediaInfoLib;
#endif

namespace umd::utils {

std::string AudioSniffMediainfo(const uint8_t* buf, size_t len) {
  std::string fmt_str("bin");

#if BUILD_WITH_MEDIAINFO
  MediaInfo mi;
  mi.Open_Buffer_Init();
  mi.Open_Buffer_Continue(buf, len);
  mi.Open_Buffer_Finalize();

  String format = mi.Get(Stream_General, 0, _T("Format"), Info_Text, Info_Name);

  fmt_str = boost::nowide::narrow(format);
  std::transform(fmt_str.begin(), fmt_str.end(), fmt_str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
#endif

  return fmt_str;
}

}  // namespace umd::utils
