//
// datum-ui - scrollarea
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
  //|---------------------- ScrollArea ----------------------------------------
  //|--------------------------------------------------------------------------

  struct ScrollArea : Item
  {
    float scrollx = 0;
    float scrolly = 0;

    float contentwidth = 0;
    float contentheight = 0;

    Sprite handleimage = nullptr;
    Color4 handletint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float handlelayer = 0.0f;
    float handleslice = 0.25f;
    float handlesize = 12;
    float handleinset = 2;

    bool wheelscroll = true;
    bool interactive = true;

    bool hoveredx = false;
    bool hoveredy = false;
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ScrollArea const &item)
;
