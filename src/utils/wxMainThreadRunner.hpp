#pragma once

#include "threading.h"

#include <wx/event.h>
#include <wx/thread.h>

#include <functional>

namespace parakeet_wx::utils
{

constexpr int kRunInMainThread = 0x10002001;
typedef std::function<void(void)> MainThreadRunnerFn;

class wxMainThreadRunner
{
  public:
    inline void SetMainThreadRunnerEventHandler(wxEvtHandler *evt_handler)
    {
        evt_handler_ = evt_handler;
    }

    inline void PostInMainThread(const MainThreadRunnerFn fn)
    {
#if defined(PARAKEET_SINGLE_THREADED)
        fn();
#else
        parakeet_wx::io_service.post([this, fn]() {
            wxThreadEvent *event = new wxThreadEvent();
            event->SetId(kRunInMainThread);
            event->SetPayload(fn);

            wxQueueEvent(evt_handler_, event);
        });
#endif
    }

    inline bool HandleMainThreadEvent(wxThreadEvent &event)
    {
        if (event.GetId() == kRunInMainThread)
        {
            auto fn = event.GetPayload<MainThreadRunnerFn>();
            fn();
            return true;
        }

        return false;
    }

  private:
    wxEvtHandler *evt_handler_;
};

} // namespace parakeet_wx::utils
