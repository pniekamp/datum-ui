//
// datum-ui - items
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "items.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;
using leap::extentof;

static constexpr float ScrollWheelSpeed = 0.05f;

namespace Ui
{
  //|---------------------- Rectangle -----------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Rectangle>(Node *node, GameInput const &input, float dt)
  {
  }

  template<>
  void Context::request_item<Rectangle>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
  }

  template<>
  void Context::prepare_item<Rectangle>(Node *node)
  {
    prepare_item<Item>(node);
  }

  template<>
  void Context::render_item<Rectangle>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    draw(spritelist, buildstate, *item_cast<Rectangle>(node));

    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- Text ----------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Text>(Node *node, GameInput const &input, float dt)
  {
  }

  template<>
  void Context::request_item<Text>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    auto item = item_cast<Text>(node);

    request(platform, fontcatalog, item->font, ready, total);
  }

  template<>
  void Context::prepare_item<Text>(Node *node)
  {
    prepare_item<Item>(node);
  }

  template<>
  void Context::render_item<Text>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    draw(spritelist, buildstate, *item_cast<Text>(node));

    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- Image ---------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Image>(Node *node, GameInput const &input, float dt)
  {
  }

  template<>
  void Context::request_item<Image>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    auto item = item_cast<Image>(node);

    request(platform, spritecatalog, item->image, ready, total);
  }

  template<>
  void Context::prepare_item<Image>(Node *node)
  {
    prepare_item<Item>(node);
  }

  template<>
  void Context::render_item<Image>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    draw(spritelist, buildstate, *item_cast<Image>(node));

    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- Frame ---------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Frame>(Node *node, GameInput const &input, float dt)
  {
  }

  template<>
  void Context::request_item<Frame>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    auto item = item_cast<Frame>(node);

    request(platform, spritecatalog, item->image, ready, total);
  }

  template<>
  void Context::prepare_item<Frame>(Node *node)
  {
    prepare_item<Item>(node);
  }

  template<>
  void Context::render_item<Frame>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    draw(spritelist, buildstate, *item_cast<Frame>(node));

    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- Row -----------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Row>(Node *node, GameInput const &input, float dt)
  {
  }

  template<>
  void Context::request_item<Row>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
  }

  template<>
  void Context::prepare_item<Row>(Node *node)
  {
    prepare_item<Item>(node);

    auto item = item_cast<Row>(node);

    auto x = 0.0f;

    for(auto child = node->firstchild; child; child = child->nextsibling)
    {
      auto childitem = item_cast<Ui::Item>(child);

      if (childitem->z == 0)
      {
        childitem->x = x;

        x += childitem->width + item->spacing;
      }
    }
  }

  template<>
  void Context::render_item<Row>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- Column --------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Column>(Node *node, GameInput const &input, float dt)
  {
  }

  template<>
  void Context::request_item<Column>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
  }

  template<>
  void Context::prepare_item<Column>(Node *node)
  {
    prepare_item<Item>(node);

    auto item = item_cast<Column>(node);

    auto y = 0.0f;

    for(auto child = node->firstchild; child; child = child->nextsibling)
    {
      auto childitem = item_cast<Ui::Item>(child);

      if (childitem->z == 0)
      {
        childitem->y = y;

        y += childitem->height + item->spacing;
      }
    }
  }

  template<>
  void Context::render_item<Column>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- ScrollBar -----------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<ScrollBar>(Node *node, GameInput const &input, float dt)
  {
    auto item = item_cast<ScrollBar>(node);

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        presspos = item->unmap(mousepos) - item->value * item->dim();

        if (presspos.x < 0 || presspos.x > item->size * item->width)
          presspos.x = 0.5f * item->size * item->width;

        if (presspos.y < 0 || presspos.y > item->size * item->height)
          presspos.y = 0.5f * item->size * item->height;

        pressitem = item;
        inputaccepted = true;
      }

      if (input.deltamousez != 0)
      {
        item->value -= input.deltamousez * ScrollWheelSpeed;

        inputaccepted = true;
      }
    }

    if (pressitem == item)
    {
      switch (item->orientation)
      {
        case Ui::ScrollBar::Horizontal:
          item->value = (item->unmap(mousepos).x - presspos.x) / item->width;
          break;

        case Ui::ScrollBar::Vertical:
          item->value = (item->unmap(mousepos).y - presspos.y) / item->height;
          break;
      }

      if (!input.mousebuttons[GameInput::Left].down())
      {
        pressitem = nullptr;
      }
    }

    item->value = clamp(item->value, 0.0f, 1 - item->size);

    item->pressed = (pressitem == item);
    item->hovered = (!inputaccepted && hoveritem == item) || (pressitem == item);
  }

  template<>
  void Context::request_item<ScrollBar>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    auto item = item_cast<ScrollBar>(node);

    request(platform, spritecatalog, item->handleimage, ready, total);
  }

  template<>
  void Context::prepare_item<ScrollBar>(Node *node)
  {
    auto item = item_cast<ScrollBar>(node);

    item->value = clamp(item->value, 0.0f, 1 - item->size);

    prepare_item<Item>(node);
  }

  template<>
  void Context::render_item<ScrollBar>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<ScrollBar>(node);

    if (item->visible && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item_cast<ScrollBar>(node));

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_rectangle ////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Rectangle const &item)
{
  if (item.opacity != 0 && item.width != 0 && item.height != 0)
  {
    if (item.color.a != 0)
    {
      spritelist.push_rect(buildstate, Vec2(item.x, item.y), Rect2(Vec2(0), Vec2(item.width, item.height)), item.rotation, item.color * item.opacity);
    }

    if (item.borderwidth != 0 && item.bordercolor.a != 0)
    {
      if (item.width > 1 && item.height > 1)
      {
        auto halfthickness = 0.5f * floor(max(item.borderwidth * item.scale, 1.0f));

        if (item.rotation != 0)
          spritelist.push_rect_outline(buildstate, Vec2(item.x, item.y), Rect2(Vec2(halfthickness, halfthickness), Vec2(item.width - halfthickness, item.height - halfthickness)), item.rotation, item.bordercolor * item.opacity, 2 * halfthickness);
        else
          spritelist.push_rect_outline(buildstate, Vec2(item.x, item.y), Rect2(Vec2(halfthickness, halfthickness), Vec2(item.width - halfthickness, item.height - halfthickness)), item.bordercolor * item.opacity, 2 * halfthickness);
      }
      else
      {
        spritelist.push_line(buildstate, Vec2(item.x, item.y), Vec2(item.x + item.width, item.y + item.height), item.bordercolor * item.opacity, item.borderwidth * item.scale);
      }
    }
  }
}


///////////////////////// draw_text /////////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Text const &item)
{
  if (item.opacity != 0 && item.width != 0 && item.height != 0 && item.text != nullptr && item.font != nullptr && item.font->ready())
  {
    auto alignment = 0;

    switch (item.horizontalalign)
    {
      case Ui::Text::HorizontalAlign::Left:
        alignment |= TextAlign::Left;
        break;

      case Ui::Text::HorizontalAlign::Center:
        alignment |= TextAlign::Center;
        break;

      case Ui::Text::HorizontalAlign::Right:
        alignment |= TextAlign::Right;
        break;

      case Ui::Text::HorizontalAlign::Justified:
        alignment |= TextAlign::Justified;
        break;
    }

    switch (item.verticalalign)
    {
      case Ui::Text::VerticalAlign::Top:
        alignment |= TextAlign::Top;
        break;

      case Ui::Text::VerticalAlign::Middle:
        alignment |= TextAlign::Middle;
        break;

      case Ui::Text::VerticalAlign::BaseLine:
        alignment |= TextAlign::BaseLine;
        break;

      case Ui::Text::VerticalAlign::Bottom:
        alignment |= TextAlign::Bottom;
        break;
    }

    draw_text(spritelist, buildstate, Vec2(item.x, item.y), Rect2(Vec2(0), Vec2(item.width, item.height)), alignment, item.rotation, item.font, item.text, item.color * item.opacity);
  }
}


///////////////////////// draw_image ////////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Image const &item)
{
  if (item.opacity != 0 && item.width != 0 && item.height != 0 && item.image != nullptr && item.image->ready())
  {
    float x = 0, y = 0, width = 0, height = 0;

    switch (item.fillmode)
    {
      case Ui::Image::Stretch:
        width = item.width;
        height = item.height;
        break;

      case Ui::Image::Fit:
        width = min(item.width, item.height * item.image->aspect);
        height = width / item.image->aspect;
        break;

      case Ui::Image::Crop:
        width = max(item.width, item.height * item.image->aspect);
        height = width / item.image->aspect;
        break;

      case Ui::Image::Pad:
        width = item.scale * item.image->width;
        height = item.scale * item.image->height;
        break;
    }

    switch (item.horizontalalign)
    {
      case Ui::Image::HorizontalAlign::Left:
        x = 0;
        break;

      case Ui::Image::HorizontalAlign::Center:
        x = 0.5f * (item.width - width);
        break;

      case Ui::Image::HorizontalAlign::Right:
        x = item.width - width;
        break;
    }

    switch (item.verticalalign)
    {
      case Ui::Image::VerticalAlign::Top:
        y = 0;
        break;

      case Ui::Image::VerticalAlign::Middle:
        y = 0.5f * (item.height - height);
        break;

      case Ui::Image::VerticalAlign::Bottom:
        y = item.height - height;
        break;
    }

    spritelist.push_sprite(buildstate, Vec2(item.x, item.y), Rect2(Vec2(x, y), Vec2(x+width, y+height)), item.rotation, item.image, item.layer, item.tint * item.opacity);
  }
}



///////////////////////// draw_frame ////////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Frame const &item)
{
  if (item.opacity != 0 && item.width != 0 && item.height != 0 && item.image != nullptr && item.image->ready())
  {
    auto xbasis1 = rotate(Vec2(min(item.slice * item.scale * item.image->width, 0.5f*item.width), 0), item.rotation);
    auto ybasis1 = rotate(Vec2(0, min(item.slice * item.scale * item.image->height, 0.5f*item.height)), item.rotation);
    auto xbasis2 = rotate(Vec2(max(item.width - 2*(item.slice * item.scale * item.image->width), 0.0f), 0), item.rotation);
    auto ybasis2 = rotate(Vec2(0, max(item.height - 2*(item.slice * item.scale * item.image->height), 0.0f)), item.rotation);

    {
      // Top-Left

      auto region = Rect2(Vec2(0, 0), Vec2(item.slice, item.slice));
      auto position = Vec2(item.x, item.y);

      spritelist.push_sprite(buildstate, position, xbasis1, ybasis1, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Top-Span

      auto region = Rect2(Vec2(item.slice, 0), Vec2(1 - item.slice, item.slice));
      auto position = Vec2(item.x, item.y) + xbasis1;

      spritelist.push_sprite(buildstate, position, xbasis2, ybasis1, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Top-Right

      auto region = Rect2(Vec2(1 - item.slice, 0), Vec2(1, item.slice));
      auto position = Vec2(item.x, item.y) + max(item.width / (item.slice * item.scale * item.image->width) - 1, 1.0f) * xbasis1;

      spritelist.push_sprite(buildstate, position, xbasis1, ybasis1, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Right-Span

      auto region = Rect2(Vec2(1 - item.slice, item.slice), Vec2(1, 1 - item.slice));
      auto position = Vec2(item.x, item.y) + max(item.width / (item.slice * item.scale * item.image->width) - 1, 1.0f) * xbasis1 + ybasis1;

      spritelist.push_sprite(buildstate, position, xbasis1, ybasis2, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Bottom-Left

      auto region = Rect2(Vec2(0, 1 - item.slice), Vec2(item.slice, 1));
      auto position = Vec2(item.x, item.y) + max(item.height / (item.slice * item.scale * item.image->height) - 1, 1.0f) * ybasis1;

      spritelist.push_sprite(buildstate, position, xbasis1, ybasis1, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Bottom-Span

      auto region = Rect2(Vec2(item.slice, 1 - item.slice), Vec2(1 - item.slice, 1));
      auto position = Vec2(item.x, item.y) + xbasis1 + max(item.height / (item.slice * item.scale * item.image->height) - 1, 1.0f) * ybasis1;

      spritelist.push_sprite(buildstate, position, xbasis2, ybasis1, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Bottom-Right

      auto region = Rect2(Vec2(1 - item.slice, 1 - item.slice), Vec2(1, 1));
      auto position = Vec2(item.x, item.y) + max(item.width / (item.slice * item.scale * item.image->width) - 1, 1.0f) * xbasis1 + max(item.height / (item.slice * item.scale * item.image->height) - 1, 1.0f) * ybasis1;

      spritelist.push_sprite(buildstate, position, xbasis1, ybasis1, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Left-Span

      auto region = Rect2(Vec2(0, item.slice), Vec2(item.slice, 1 - item.slice));
      auto position = Vec2(item.x, item.y) + ybasis1;

      spritelist.push_sprite(buildstate, position, xbasis1, ybasis2, item.image, region, item.layer, item.tint * item.opacity);
    }

    {
      // Center

      auto region = Rect2(Vec2(item.slice, item.slice), Vec2(1 - item.slice, 1 - item.slice));
      auto position = Vec2(item.x, item.y) + xbasis1 + ybasis1;

      spritelist.push_sprite(buildstate, position, xbasis2, ybasis2, item.image, region, item.layer, item.tint * item.opacity);
    }
  }
}


///////////////////////// draw_scrollbar ////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ScrollBar const &item)
{
  auto size = item.size;

  switch (item.orientation)
  {
    case Ui::ScrollBar::Horizontal:
      size = max(size, 20*item.scale/item.width);
      break;

    case Ui::ScrollBar::Vertical:
      size = max(size, 20*item.scale/item.height);
      break;
  }

  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto handleinset = item.scale * item.handleinset;

  float value = clamp(item.value, 0.0f, 1 - size);

  if (item.handleimage)
  {
    Ui::Frame handle;
    handle.x = item.x;
    handle.y = item.y;
    handle.width = item.width;
    handle.height = item.height;
    handle.scale = item.scale;
    handle.rotation = item.rotation;
    handle.opacity = item.opacity;
    handle.image = item.handleimage;
    handle.tint = item.handletint;
    handle.layer = item.handlelayer;
    handle.slice = item.handleslice;
    handle.scale = item.scale;

    float x = 0, y = 0, width = 0, height = 0;

    switch (item.orientation)
    {
      case Ui::ScrollBar::Horizontal:
        x = value * item.width;
        y = handleinset;
        width = size * item.width;
        height = item.height - 2*handleinset;
        break;

      case Ui::ScrollBar::Vertical:
        x = item.width - handleinset;
        y = value * item.height;
        width = size * item.height;
        height = item.width - 2*handleinset;
        handle.rotation += pi<float>()/2;
        break;
    }

    handle.x += x*xbasis.x + y*ybasis.x;
    handle.y += x*xbasis.y + y*ybasis.y;
    handle.width = width;
    handle.height = height;

    draw(spritelist, buildstate, handle);
  }

  if (!item.handleimage)
  {
    float x = 0, y = 0, width = 0, height = 0;

    switch (item.orientation)
    {
      case Ui::ScrollBar::Horizontal:
        x = value * item.width;
        y = handleinset;
        width = size * item.width;
        height = item.height - 2*handleinset;
        break;

      case Ui::ScrollBar::Vertical:
        x = handleinset;
        y = value * item.height;
        width = item.width - 2*handleinset;
        height = size * item.height;
        break;
    }

    Ui::Rectangle handle;
    handle.x = floor(item.x + x*xbasis.x + y*ybasis.x);
    handle.y = floor(item.y + x*xbasis.y + y*ybasis.y);
    handle.width = width;
    handle.height = height;
    handle.rotation = item.rotation;
    handle.opacity = item.opacity;
    handle.color = item.handletint;

    draw(spritelist, buildstate, handle);
  }
}
