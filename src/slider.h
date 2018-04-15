//
// datum-ui - slider
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/math.h"
#include "datum/memory.h"
#include "control.h"

namespace Ui
{ 
  //|---------------------- Slider --------------------------------------------
  //|--------------------------------------------------------------------------

  struct Slider : Control
  {
    float value = 0;

    float minvalue = 0.0f;
    float maxvalue = 1.0f;

    enum Orientation
    {
      Horizontal,
      Vertical,
    };

    int orientation = Horizontal;

    float inset = 0.0f;

    Sprite handleimage = nullptr;
    Color4 handletint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float handlelayer = 0.0f;

    bool pressed = false;

    enum Op
    {
      Pressed,
      Released,
      Dragged,
    };

    Slider() : Control() { backgroundcolor = Color4(0.0f, 0.0f, 0.0f, 0.0f); }
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Slider const &item);
