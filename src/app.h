#pragma once

#include "pch.h"

class ParakeetWx : public wxApp
{
  public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
};

DECLARE_APP(ParakeetWx)
