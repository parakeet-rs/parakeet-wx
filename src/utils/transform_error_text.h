#include <parakeet-crypto/IStream.h>

#include <wx/translation.h>

namespace parakeet_wx
{

inline const wxString to_string(parakeet_crypto::TransformResult result)
{
    using parakeet_crypto::TransformResult;

    switch (result)
    {
    case TransformResult::OK:
        return wxT("OK");
    case TransformResult::ERROR_OTHER:
        return wxT("ERROR_OTHER");
    case TransformResult::ERROR_INSUFFICIENT_OUTPUT:
        return wxT("ERROR_INSUFFICIENT_OUTPUT");
    case TransformResult::ERROR_INVALID_FORMAT:
        return wxT("ERROR_INVALID_FORMAT");
    case TransformResult::ERROR_INVALID_KEY:
        return wxT("ERROR_INVALID_KEY");
    case TransformResult::ERROR_INSUFFICIENT_INPUT:
        return wxT("ERROR_INSUFFICIENT_INPUT");
    case TransformResult::ERROR_IO_OUTPUT_UNKNOWN:
        return wxT("ERROR_IO_OUTPUT_UNKNOWN");
    case TransformResult::ERROR_NOT_IMPLEMENTED:
        return wxT("ERROR_NOT_IMPLEMENTED");
    default:
        return wxString::Format("ERROR_UNKNOWN(%d)", static_cast<int>(result));
    }
}

} // namespace parakeet_wx
