//
// datum-ui - slideedit
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "slideedit.h"
#include "context.h"
#include "textarea.h"
#include "button.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

static constexpr float MinDragDistance = 3.0f;

namespace Ui
{
  //|---------------------- SlideEdit -----------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<SlideEdit>(Node *node, GameInput const &input, float dt)
  {
    auto item = item_cast<SlideEdit>(node);

    if (node->strings.empty())
      node->strings.resize(1);

    if (focusitem != item)
    {
      char buffer[128];
      snprintf(buffer, sizeof(buffer), item->format, item->precision, item->value);

      node->strings[0] = buffer;
    }

    if (pressitem != item)
    {
      item->hoveredhandle = false;
    }

    if (!inputaccepted && hoveritem == item)
    {
      auto mouse = item->unmap(mousepos);
      auto padding = item->scale * item->padding;
      auto handlewidth = (item->handleimage) ? item->scale * item->handleimage->width : 1.4f*item->fontheight;
      auto handleheight = (item->handleimage) ? item->scale * item->handleimage->height : item->fontheight;

      if (mouse.x > item->width - padding - handlewidth && mouse.x < item->width - padding && mouse.y > item->height/2 - handleheight/2 && mouse.y < item->height/2 + handleheight/2)
        item->hoveredhandle = item->hovered = true;

      if (item->hoveredhandle)
      {
        if (input.mousebuttons[GameInput::Left].pressed())
        {
          dragging = false;
          presspos = item->unmap(mousepos);

          focusitem = item;
          pressitem = item;
          inputaccepted = true;
        }
      }

      if (input.deltamousez != 0)
      {
        item->value = clamp(item->value + input.deltamousez * item->sliderate, item->minvalue, item->maxvalue);

        char buffer[128];
        snprintf(buffer, sizeof(buffer), item->format, item->precision, item->value);

        actions.push_back(Action{ item->action, SlideEdit::Changed, ui, item });

        node->strings[0] = buffer;

        item->selectionbeg = 0;
        item->selectionend = item->cursor = node->strings[0].length();

        focusitem = item;
        inputaccepted = true;
      }
    }

    if (item->hoveredhandle && pressitem == item)
    {
      if (!dragging)
      {
        if (dist(item->map(presspos), mousepos) > MinDragDistance)
        {
          dragging = true;
        }
      }

      if (dragging)
      {
        item->value = clamp(item->value + 100 * input.deltamousex * item->sliderate, item->minvalue, item->maxvalue);

        char buffer[128];
        snprintf(buffer, sizeof(buffer), item->format, item->precision, item->value);

        actions.push_back(Action{ item->action, SlideEdit::Changed, ui, item });

        node->strings[0] = buffer;

        item->selectionbeg = 0;
        item->selectionend = item->cursor = node->strings[0].length();
        item->focused = true;

        cursor = Cursor::Hide;
      }

      if (!input.mousebuttons[GameInput::Left].down())
      {
        pressitem = nullptr;
      }
    }

    item->text = node->strings[0].c_str();

    if (!(item->hoveredhandle && pressitem == item))
    {
      update_item<LineEdit>(node, input, dt);

      item->value = clamp(strtof(item->text, nullptr), item->minvalue, item->maxvalue);
    }
  }


  ///////////////////////// request_item //////////////////////////////////////
  template<>
  void Context::request_item<SlideEdit>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    request_item<LineEdit>(platform, node, ready, total);
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<SlideEdit>(Node *node)
  {
    prepare_item<LineEdit>(node);
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<SlideEdit>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<SlideEdit>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item_cast<SlideEdit>(node));

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_slideedit ////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::SlideEdit const &item)
{
  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto padding = item.scale * item.padding;
  auto handlewidth = (item.handleimage) ? item.scale * item.handleimage->width : 1.4f*item.fontheight;
  auto handleheight = (item.handleimage) ? item.scale * item.handleimage->height : item.fontheight;

  draw(spritelist, buildstate, static_cast<Ui::LineEdit const &>(item));

  Ui::Button handle;
  handle.x = item.x + (item.width - padding - handlewidth) * xbasis.x + (item.height - handleheight)/2 * ybasis.x;
  handle.y = item.y + (item.width - padding - handlewidth) * xbasis.y + (item.height - handleheight)/2 * ybasis.y;
  handle.width = handlewidth;
  handle.height = handleheight;
  handle.scale = item.scale;
  handle.rotation = item.rotation;
  handle.opacity = (item.hoveredhandle ? 1.0f : 0.5f) * item.opacity;
  handle.font = item.font;
  handle.fontheight = item.fontheight;
  handle.image = item.handleimage;
  handle.imagetint = item.handleimagetint;
  handle.imagelayer = item.handleimagelayer;
  handle.text = (!item.handleimage) ? "<>" : nullptr;
  handle.textcolor = item.textcolor;

  draw(spritelist, buildstate, handle);
}
