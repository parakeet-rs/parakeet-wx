#pragma once

#include "pch.h"

class umDesktopApp : public wxApp {
 public:
  virtual bool OnInit() override;
  virtual int OnExit() override;
};

DECLARE_APP(umDesktopApp)
