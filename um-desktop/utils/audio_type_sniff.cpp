#include "audio_type_sniff.h"

#include <boost/nowide/convert.hpp>

#include <wx/window.h>

#if USE_MEDIAINFO_SNIFF
#include <MediaInfo/MediaInfo.h>
using MediaInfoLib::MediaInfo;
#endif

namespace nowide = boost::nowide;

std::string SniffAudioType(uint8_t* buf, size_t len) {
#if USE_MEDIAINFO_SNIFF
  MediaInfo mi;
  mi.Open_Buffer_Init();
  mi.Open_Buffer_Continue(buf, len);
  mi.Open_Buffer_Finalize();

  MediaInfoLib::String format =
      mi.Get(MediaInfoLib::Stream_General, 0, _T("Format"),
             MediaInfoLib::Info_Text, MediaInfoLib::Info_Name);

  auto fmt_str = boost::nowide::narrow(format);

  std::transform(fmt_str.begin(), fmt_str.end(), fmt_str.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return fmt_str;
#endif

  return "bin";
}
