//
// datum-ui - label
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
  //|---------------------- Label ---------------------------------------------
  //|--------------------------------------------------------------------------

  struct Label : Control
  {
    const char *text = nullptr;

    Color4 textcolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    enum HorizontalAlign
    {
      Left,
      Center,
      Right,
      Justified
    };

    int horizontalalign = Center;

    enum VerticalAlign
    {
      Top,
      Middle,
      Bottom,
      BaseLine,
    };

    int verticalalign = Middle;

    float padding = 0.0f;
  };


  //|---------------------- Field ---------------------------------------------
  //|--------------------------------------------------------------------------

  struct Field : Control
  {
    float value = 0;

    int precision = 6;
    const char *format = "%.*f";

    Color4 textcolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    enum HorizontalAlign
    {
      Left,
      Center,
      Right
    };

    int horizontalalign = Center;

    enum VerticalAlign
    {
      Top,
      Middle,
      Bottom,
      BaseLine,
    };

    int verticalalign = Middle;

    float padding = 0.0f;
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Label const &item);
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Field const &item);
