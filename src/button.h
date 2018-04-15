//
// datum-ui - button
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
  //|---------------------- Button -------------------------------------------
  //|--------------------------------------------------------------------------

  struct Button : Control
  {
    const char *text = nullptr;

    Color4 textcolor = { 0.0f, 0.0f, 0.0f, 1.0f };

    enum TextAlign
    {
      Left,
      Center,
      Right,
    };

    int textalign = Center;

    Sprite image = nullptr;
    Color4 imagetint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float imagelayer = 0.0f;

    enum ImageAlign
    {
      Fill,
      Before,
      After,
      Above,
      Below
    };

    int imagealign = Fill;

    float spacing = 0.0f;
    float padding = 0.0f;

    bool pressed = false;

    enum Op
    {
      Pressed,
      Released,
      Clicked,
      DoubleClicked,
    };
  };


  //|---------------------- CheckButton ---------------------------------------
  //|--------------------------------------------------------------------------

  struct CheckButton : Button
  {
    enum CheckState
    {
      UnChecked,
      Indeterminate,
      Checked,
    };

    int checked = Indeterminate;

    Color4 innercolor = { 0.0f, 0.0f, 1.0f, 0.0f };
    Color4 outercolor = { 0.0f, 0.0f, 0.0f, 0.0f };

    enum Op
    {
      Pressed,
      Released,
      Clicked,
    };

    CheckButton() : Button() { textalign = Left; imagealign = Before; padding = 5.0f; }
  };


  //|---------------------- DragButton ----------------------------------------
  //|--------------------------------------------------------------------------

  struct DragButton : Button
  {
    float dx = 0.0f;
    float dy = 0.0f;

    enum Op
    {
      Pressed,
      Released,
      Clicked,
      DoubleClicked,
      Dragged,
    };
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Button const &item);
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::CheckButton const &item);
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::DragButton const &item);
