//
// datum-ui - listbox
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
  //|---------------------- ListBox -------------------------------------------
  //|--------------------------------------------------------------------------

  struct ListBox : Control
  {
    const char *text = nullptr;

    Color4 textcolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    int selection = -1;

    Color4 selectioncolor = { 0.04f, 0.36f, 1.0f, 1.0f };
    Color4 selectiontextcolor = { 1.0f, 1.0f, 1.0f, 1.0f };

    Sprite handleimage = nullptr;
    Color4 handletint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float handlelayer = 0.0f;
    float handleslice = 0.25f;
    float handlesize = 12;
    float handleinset = 2;

    float padding = 4.0f;

    bool focused = false;   

    enum Op
    {
      Pressed,
      Released,
      Selected,
    };

    float scrollx = 0;
    float scrolly = 0;
    float contentheight = 0;

    ListBox() : Control() { borderwidth = 1; bordercolor = lml::Color4(0.4f, 0.4f, 0.4f, 1.0f); }
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ListBox const &item);
