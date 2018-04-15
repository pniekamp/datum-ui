//
// datum-ui - control
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/math.h"
#include "datum/memory.h"
#include "items.h"

namespace Ui
{ 
  //|---------------------- Control -------------------------------------------
  //|--------------------------------------------------------------------------

  struct Control : Item
  {
    int borderwidth = 0;
    Color4 bordercolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    Sprite backgroundimage = nullptr;
    Color4 backgroundcolor = { 0.0f, 0.0f, 0.0f, 0.0f };
    float backgroundlayer = 0.0f;
    float backgroundslice = 0.25f;

    Font font = nullptr;
    float fontheight = 0.0f;

    bool enabled = true;
    bool hovered = false;

    int action = 0;
  };
}
