#pragma once

#include "pch.h"

class umDesktopApp : public wxApp {
 public:
  virtual bool OnInit();
};

DECLARE_APP(umDesktopApp)
