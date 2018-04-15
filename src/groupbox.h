//
// datum-ui - groupbox
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/math.h"
#include "datum/memory.h"
#include "button.h"

namespace Ui
{ 
  //|---------------------- GroupBox ------------------------------------------
  //|--------------------------------------------------------------------------

  struct GroupBox : CheckButton
  {
    enum Op
    {
      Pressed,
      Released,
      Clicked,
    };

    GroupBox() : CheckButton() { checked = CheckButton::UnChecked; spacing = 2.0f; }
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::GroupBox const &item);
