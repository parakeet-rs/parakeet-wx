#pragma once

#include "config/AppConfigStore.h"
#include "ui/ui.h"

#include <utility>

class OptionsDialog : public parakeet_wx::ui_base::uiOptionsDialog
{
  public:
    OptionsDialog(wxWindow *parent);
    inline const parakeet_wx::config::AppConfig &GetSavedConfig() const
    {
        return saved_config_;
    };
    inline bool IsConfigSaved() const
    {
        return saved_;
    };

    virtual void OnButtonClick_Cancel(wxCommandEvent &event) override;
    virtual void OnButtonClick_OK(wxCommandEvent &event) override;

  private:
    template <int M> void ConfigGlueCode();

    bool saved_ = false;
    parakeet_wx::config::AppConfig saved_config_;
};
