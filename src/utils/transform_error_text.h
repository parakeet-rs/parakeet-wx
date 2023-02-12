#include <parakeet-crypto/IStream.h>

#include <wx/translation.h>

namespace parakeet_wx
{

inline const wxString &to_string(parakeet_crypto::TransformResult result)
{
    using parakeet_crypto::TransformResult;

    switch (result)
    {
    case TransformResult::OK:
        return _("OK");
    case TransformResult::ERROR_OTHER:
        return _("ERROR_OTHER");
    case TransformResult::ERROR_INSUFFICIENT_OUTPUT:
        return _("ERROR_INSUFFICIENT_OUTPUT");
    case TransformResult::ERROR_INVALID_FORMAT:
        return _("ERROR_INVALID_FORMAT");
    case TransformResult::ERROR_INVALID_KEY:
        return _("ERROR_INVALID_KEY");
    case TransformResult::ERROR_INSUFFICIENT_INPUT:
        return _("ERROR_INSUFFICIENT_INPUT");
    case TransformResult::ERROR_IO_OUTPUT_UNKNOWN:
        return _("ERROR_IO_OUTPUT_UNKNOWN");
    case TransformResult::ERROR_NOT_IMPLEMENTED:
        return _("ERROR_NOT_IMPLEMENTED");
    default:
        return _("");
    }
}

} // namespace parakeet_wx
