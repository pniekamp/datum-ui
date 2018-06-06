//
// datum-ui - slideedit
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/math.h"
#include "datum/memory.h"
#include "lineedit.h"

namespace Ui
{ 
  //|---------------------- SlideEdit -----------------------------------------
  //|--------------------------------------------------------------------------

  struct SlideEdit : LineEdit
  {
    float value;

    int precision = 2;
    const char *format = "%.*f";

    float minvalue = std::numeric_limits<float>::lowest();
    float maxvalue = std::numeric_limits<float>::max();

    Sprite handleimage = nullptr;
    Color4 handleimagetint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float handleimagelayer = 0.0f;

    float sliderate = 0.1f;

    enum Op
    {
      Focused,
      Changed,
      Uped,
      Downed,
      Tabbed,
      Canceled,
      Accepted,
    };

    bool hoveredhandle = false;

    SlideEdit() : LineEdit() { filter = LineEdit::Decimal; }
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::SlideEdit const &item);
