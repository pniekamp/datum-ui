//
// datum-ui - lineedit
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
  //|---------------------- LineEdit ------------------------------------------
  //|--------------------------------------------------------------------------

  struct LineEdit : Control
  {
    const char *text = nullptr;

    Color4 textcolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    int cursor = -1;
    int selectionbeg = 0;
    int selectionend = 0;

    Color4 selectioncolor = { 0.04f, 0.36f, 1.0f, 1.0f };
    Color4 selectiontextcolor = { 1.0f, 1.0f, 1.0f, 1.0f };

    float padding = 4.0f;

    bool focused = false;

    enum Filter
    {
      Alpha = 0x01,  // A..Z a..z
      Digit = 0x02, // 0..9
      Integer = 0x04, // 0..9 + -
      Decimal = 0x08, // 0..9 + - . e E
      Punct = 0x10,
      Space = 0x20,
      Paste = 0x40,
      Extended = 0x80, // Unicode

      Text = 0xFFFF,
      Readonly = 0x0000
    };

    int filter = Text;

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

    float scrollx = 0;
    float scrolly = 0;

    LineEdit() : Control() { borderwidth = 1; bordercolor = lml::Color4(0.4f, 0.4f, 0.4f, 1.0f); }
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::LineEdit const &item);
