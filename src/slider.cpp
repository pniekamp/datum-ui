//
// datum-ui - slider
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "slider.h"
#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

static constexpr float ScrollWheelSpeed = 0.05f;

namespace Ui
{
  //|---------------------- Slider --------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Slider>(Node *node, DatumPlatform::GameInput const &input, float dt)
  {   
    update_item<Control>(node, input, dt);

    auto item = item_cast<Slider>(node);

    auto handlewidth = item->scale * ((item->handleimage) ? item->handleimage->width : 8);
    auto handleheight = item->scale * ((item->handleimage) ? item->handleimage->height : 8);

    float value = (item->value - item->minvalue) / (item->maxvalue - item->minvalue);

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        presspos = item->unmap(mousepos) - item->value * Vec2(item->width - handlewidth, item->height - handleheight);

        if (presspos.x < 0 || presspos.x > handlewidth)
          presspos.x = 0.5f * handlewidth;

        if (presspos.y < 0 || presspos.y > handleheight)
          presspos.y = 0.5f * handleheight;

        actions.push_back(Action{ item->action, Slider::Pressed, ui, item });

        pressitem = item;
      }

      if (input.deltamousez != 0)
      {
        value -= input.deltamousez * ScrollWheelSpeed;

        inputaccepted = true;
      }
    }

    if (pressitem == item)
    {
      switch (item->orientation)
      {
        case Ui::Slider::Horizontal:
          value = (item->unmap(mousepos).x - presspos.x) / (item->width - handlewidth);
          break;

        case Ui::Slider::Vertical:
          value = (item->unmap(mousepos).y - presspos.y) / (item->height - handleheight);
          break;
      }

      actions.push_back(Action{ item->action, Slider::Dragged, ui, item });

      if (!input.mousebuttons[GameInput::Left].down())
      {
        actions.push_back(Action{ item->action, Slider::Released, ui, item });

        pressitem = nullptr;
      }

      inputaccepted = true;
    }

    item->value = item->minvalue + clamp(value, 0.0f, 1.0f) * (item->maxvalue - item->minvalue);

    item->pressed = (pressitem == item);
  }

  template<>
  void Context::prepare_item<Slider>(Node *node)
  {
    prepare_item<Control>(node);

    auto item = item_cast<Slider>(node);

    item->value = clamp(item->value, item->minvalue, item->maxvalue);
  }

  template<>
  void Context::render_item<Slider>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<Slider>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item);

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_slider ///////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Slider const &item)
{
  auto handlewidth = 8;
  auto handleheight = 16;

  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto inset = item.scale * item.inset;

  Ui::Rectangle background;
  background.x = item.x;
  background.y = item.y;
  background.width = item.width;
  background.height = item.height;
  background.scale = item.scale;
  background.rotation = item.rotation;
  background.opacity = item.opacity;
  background.color = item.backgroundcolor;
  background.borderwidth = item.borderwidth;
  background.bordercolor = item.bordercolor;

  switch (item.orientation)
  {
    case Ui::Slider::Horizontal:
      background.x += inset * ybasis.x;
      background.y += inset * ybasis.y;
      background.height -= 2 * inset;
      break;

    case Ui::Slider::Vertical:
      background.x += inset * xbasis.x;
      background.y += inset * xbasis.y;
      background.width -= 2 * inset;
      break;
  }

  draw(spritelist, buildstate, background);

  Ui::Frame frame;
  frame.x = item.x;
  frame.y = item.y;
  frame.width = item.width;
  frame.height = item.height;
  frame.scale = item.scale;
  frame.rotation = item.rotation;
  frame.opacity = item.opacity;
  frame.image = item.backgroundimage;
  frame.layer = item.backgroundlayer;
  frame.slice = item.backgroundslice;

  switch (item.orientation)
  {
    case Ui::Slider::Horizontal:
      frame.x += inset * ybasis.x;
      frame.y += inset * ybasis.y;
      frame.height -= 2 * inset;
      break;

    case Ui::Slider::Vertical:
      frame.x += item.width * xbasis.x - inset * xbasis.x;
      frame.y += item.width * xbasis.y - inset * xbasis.y;
      frame.width -= 2 * inset;
      swap(frame.width, frame.height);
      frame.rotation += pi<float>()/2;
      break;
  }

  draw(spritelist, buildstate, frame);

  float value = clamp((item.value - item.minvalue) / (item.maxvalue - item.minvalue), 0.0f, 1.0f);

  if (item.handleimage)
  {
    Ui::Image handle;
    handle.x = item.x;
    handle.y = item.y;
    handle.width = item.scale * item.handleimage->width;
    handle.height = item.scale * item.handleimage->height;
    handle.scale = item.scale;
    handle.rotation = item.rotation;
    handle.opacity = item.opacity;
    handle.image = item.handleimage;
    handle.tint = item.handletint;
    handle.layer = item.handlelayer;
    handle.scale = item.scale;
    handle.fillmode = Ui::Image::Pad;

    switch (item.orientation)
    {
      case Ui::Slider::Horizontal:
        handle.x += value * (item.width - handle.width) * xbasis.x + 0.5f * (item.height - handle.height) * ybasis.x;
        handle.y += value * (item.width - handle.width) * xbasis.y + 0.5f * (item.height - handle.height) * ybasis.y;
        break;

      case Ui::Slider::Vertical:
        handle.x += 0.5f * (item.width + handle.height) * xbasis.x + value * (item.height - handle.width) * ybasis.x;
        handle.y += 0.5f * (item.width + handle.height) * xbasis.y + value * (item.height - handle.width) * ybasis.y;
        handle.rotation += pi<float>()/2;
        break;
    }

    draw(spritelist, buildstate, handle);
  }

  if (!item.handleimage)
  {
    float x = 0, y = 0, width = 0, height = 0;

    switch (item.orientation)
    {
      case Ui::Slider::Horizontal:
        x = value * (item.width - item.scale * handlewidth);
        y = 0.5f * (item.height - item.scale * handleheight);
        width = item.scale * handlewidth;
        height = item.scale * handleheight;
        break;

      case Ui::Slider::Vertical:
        x = 0.5f * (item.width - item.scale * handleheight);
        y = value * (item.height - item.scale * handlewidth);
        width = item.scale * handleheight;
        height = item.scale * handlewidth;
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
    handle.borderwidth = 1;
    handle.bordercolor = Color4(0.4f, 0.4f, 0.4f, 1);

    draw(spritelist, buildstate, handle);
  }
}
