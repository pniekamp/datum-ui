//
// datum-ui - combobox
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
  //|---------------------- ComboBox ------------------------------------------
  //|--------------------------------------------------------------------------

  struct ComboBox : Control
  {
    int selection = -1;

    const char *text = nullptr;

    Color4 textcolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    Sprite dropimage = nullptr;
    Color4 dropimagetint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float dropimagelayer = 0.0f;

    int listborderwidth = 1;
    Color4 listbordercolor = { 0.4f, 0.4f, 0.40f, 1.0f };
    Color4 listbackgroundcolor = { 1.0f, 1.0f, 1.0f, 1.0f };

    Color4 selectioncolor = { 0.04f, 0.36f, 1.0f, 1.0f };
    Color4 selectiontextcolor = { 1.0f, 1.0f, 1.0f, 1.0f };

    Sprite handleimage = nullptr;
    Color4 handletint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float handlelayer = 0.0f;
    float handleslice = 0.25f;
    float handlesize = 12;
    float handleinset = 2;

    float padding = 5.0f;

    bool pressed = false;
    bool dropped = false;

    enum Op
    {
      Pressed,
      Released,
      Changed,
    };
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ComboBox const &item);
