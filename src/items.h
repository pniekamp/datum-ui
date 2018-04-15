//
// datum-ui - items
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/math.h"
#include "datum/memory.h"
#include "context.h"

namespace Ui
{
  using Color3 = lml::Color3;
  using Color4 = lml::Color4;

  //|---------------------- Rectangle -----------------------------------------
  //|--------------------------------------------------------------------------

  struct Rectangle : Item
  {
    Color4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    int borderwidth = 0;
    Color4 bordercolor = { 1.0f, 1.0f, 1.0f, 1.0f };
  };


  //|---------------------- Text ----------------------------------------------
  //|--------------------------------------------------------------------------

  struct Text : Item
  {
    const char *text = nullptr;

    Font font = nullptr;
    Color4 color = { 0.0f, 0.0f, 0.0f, 1.0f };

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
  };


  //|---------------------- Image ---------------------------------------------
  //|--------------------------------------------------------------------------

  struct Image : Item
  {
    Sprite image = nullptr;
    Color4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float layer = 0.0f;

    enum FillMode
    {
      Stretch,
      Fit,
      Crop,
      Pad
    };

    int fillmode = Stretch;

    enum HorizontalAlign
    {
      Left,
      Center,
      Right,
    };

    int horizontalalign = Center;

    enum VerticalAlign
    {
      Top,
      Middle,
      Bottom,
    };

    int verticalalign = Middle;
  };


  //|---------------------- Frame ---------------------------------------------
  //|--------------------------------------------------------------------------

  struct Frame : Item
  {
    Sprite image = nullptr;
    Color4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float layer = 0.0f;
    float slice = 0.25f;
  };


  //|---------------------- Row -----------------------------------------------
  //|--------------------------------------------------------------------------

  struct Row : Item
  {
    float spacing = 0.0f;
  };


  //|---------------------- Column --------------------------------------------
  //|--------------------------------------------------------------------------

  struct Column : Item
  {
    float spacing = 0.0f;
  };


  //|---------------------- ScrollBar -----------------------------------------
  //|--------------------------------------------------------------------------

  struct ScrollBar : Item
  {
    float value = 0;

    float size = 1.0f;

    enum Orientation
    {
      Horizontal,
      Vertical,
    };

    int orientation = Vertical;

    Sprite handleimage = nullptr;
    Color4 handletint = { 1.0f, 1.0f, 1.0f, 1.0f };
    float handlelayer = 0.0f;
    float handleslice = 0.25f;
    float handleinset = 0.0f;

    bool hovered = false;
    bool pressed = false;
  };
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Rectangle const &item);
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Text const &item);
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Image const &item);
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Frame const &item);
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ScrollBar const &item);
