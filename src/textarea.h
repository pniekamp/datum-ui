//
// datum-ui - textarea
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
  //|---------------------- TextArea ------------------------------------------
  //|--------------------------------------------------------------------------

  struct TextArea : Item
  {
    const char *text = nullptr;

    Font font = nullptr;
    Color4 textcolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    enum HorizontalAlign
    {
      Left,
      Center,
      Right,
      Justified
    };

    int horizontalalign = Left;

    enum VerticalAlign
    {
      Top,
      Middle,
      Bottom,
    };

    int verticalalign = Top;

    int cursor = -1;
    int selectionbeg = 0;
    int selectionend = 0;

    Color4 selectioncolor = { 0.04f, 0.36f, 1.0f, 1.0f };
    Color4 selectiontextcolor = { 1.0f, 1.0f, 1.0f, 1.0f };

    bool interactive = false;

  public:

    enum Position
    {
      Start,
      CharLeft,
      CharRight,
      WordLeft,
      WordRight,
      LineUp,
      LineDown,
      LineStart,
      LineEnd,
      End,
    };

    int position(int from, Position to);
    int position(lml::Vec2 const &worldpos);

    lml::Vec2 position(int index);
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::TextArea const &item);
