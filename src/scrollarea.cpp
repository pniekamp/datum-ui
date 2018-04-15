//
// datum-ui - scrollarea
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "scrollarea.h"
#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

static constexpr float ScrollWheelSpeed = 25.0f;

namespace Ui
{
  //|---------------------- ScrollArea ----------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<ScrollArea>(Node *node, GameInput const &input, float dt)
  {
    update_item<Item>(node, input, dt);

    auto item = item_cast<ScrollArea>(node);

    if (pressitem != item)
    {
      item->hoveredx = false;
      item->hoveredy = false;
    }

    if (!inputaccepted && hoveritem == item)
    {
      auto mouse = item->unmap(mousepos);
      auto handlesize = item->scale * item->handlesize;

      if (item->width < item->contentwidth)
      {
        if (mouse.x > 0 && mouse.x < item->width && mouse.y > item->height - handlesize && mouse.y < item->height)
          item->hoveredx = true;
      }

      if (item->height < item->contentheight)
      {
        if (mouse.x > item->width - handlesize && mouse.x < item->width && mouse.y > 0 && mouse.y < item->height)
          item->hoveredy = true;
      }

      if (input.mousebuttons[GameInput::Left].pressed())
      {
        presspos = mouse - Vec2(item->scrollx / item->contentwidth * item->width, item->scrolly / item->contentheight * item->height);

        if (presspos.x < 0 || presspos.x > item->width / item->contentwidth * item->width)
          presspos.x = 0.5f * item->width / item->contentwidth * item->width;

        if (presspos.y < 0 || presspos.y > item->height / item->contentheight * item->height)
          presspos.y = 0.5f * item->height / item->contentheight * item->height;

        pressitem = item;
      }

      if (input.deltamousez != 0)
      {
        item->scrolly -= input.deltamousez * ScrollWheelSpeed;

        inputaccepted = true;
      }
    }

    if (pressitem == item)
    {
      if (item->hoveredx)
      {
        item->scrollx = round((item->unmap(mousepos).x - presspos.x) / item->width * item->contentwidth);
      }

      if (item->hoveredy)
      {
        item->scrolly = round((item->unmap(mousepos).y - presspos.y) / item->height * item->contentheight);
      }

      if (!input.mousebuttons[GameInput::Left].down())
      {
        pressitem = nullptr;
      }

      inputaccepted = true;
    }

    item->scrollx = clamp(item->scrollx, 0.0f, item->contentwidth - item->width);
    item->scrolly = clamp(item->scrolly, 0.0f, item->contentheight - item->height);
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<ScrollArea>(Node *node)
  {
    prepare_item<Item>(node);

    auto item = item_cast<ScrollArea>(node);

    item->contentwidth = item->scale * item->contentwidth;
    item->contentheight = item->scale * item->contentheight;

    item->scrollx = clamp(item->scrollx, 0.0f, item->contentwidth - item->width);
    item->scrolly = clamp(item->scrolly, 0.0f, item->contentheight - item->height);

    auto dx = -item->scrollx / item->scale;
    auto dy = -item->scrolly / item->scale;

    for(auto child = node->firstchild; child; child = child->nextsibling)
    {
      auto childitem = item_cast<Ui::Item>(child);

      childitem->x += dx;
      childitem->y += dy;
    }
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<ScrollArea>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<ScrollArea>(node);

    if (item->visible && item->interactive && item->wheelscroll && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    for(auto child = node->firstchild; child; child = child->nextsibling)
    {
      render(spritelist, buildstate, child);
    }

    if (item->visible && item->interactive && contains(buildstate.cliprect(), mousepos))
    {
      auto mouse = item->unmap(mousepos);

      auto handlesize = item->scale * item->handlesize;

      if (item->width < item->contentwidth)
      {
        if (mouse.x > 0 && mouse.x < item->width && mouse.y > item->height - handlesize && mouse.y < item->height)
          hoveritem = item;
      }

      if (item->height < item->contentheight)
      {
        if (mouse.x > item->width - handlesize && mouse.x < item->width && mouse.y > 0 && mouse.y < item->height)
          hoveritem = item;
      }
    }

    draw(spritelist, buildstate, *item_cast<ScrollArea>(node));
  }
}


///////////////////////// draw_scrollarea ///////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ScrollArea const &item)
{
  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto handlesize = item.scale * item.handlesize;

  auto cornerpad = (item.width < item.contentwidth && item.height < item.contentheight) ? handlesize : 0.0f;

  if (item.width < item.contentwidth)
  {
    Ui::ScrollBar horizontal;
    horizontal.x = item.x + (item.height - handlesize) * ybasis.x;
    horizontal.y = item.y + (item.height - handlesize) * ybasis.y;
    horizontal.width = item.width - cornerpad;
    horizontal.height = handlesize;
    horizontal.scale = item.scale;
    horizontal.rotation = item.rotation;
    horizontal.opacity = item.opacity;
    horizontal.orientation = Ui::ScrollBar::Horizontal;
    horizontal.handleimage = item.handleimage;
    horizontal.handletint = item.handletint;
    horizontal.handlelayer = item.handlelayer;
    horizontal.handleslice = item.handleslice;
    horizontal.handleinset = item.handleinset;
    horizontal.size = item.width / item.contentwidth;
    horizontal.value = item.scrollx / item.contentwidth;

    if (item.hoveredx)
      horizontal.handletint.a *= 2.0f;

    draw(spritelist, buildstate, horizontal);
  }

  if (item.height < item.contentheight)
  {
    Ui::ScrollBar vertical;
    vertical.x = item.x + (item.width - handlesize) * xbasis.x;
    vertical.y = item.y + (item.width - handlesize) * xbasis.y;
    vertical.width = handlesize;
    vertical.height = item.height - cornerpad;
    vertical.scale = item.scale;
    vertical.rotation = item.rotation;
    vertical.opacity = item.opacity;
    vertical.orientation = Ui::ScrollBar::Vertical;
    vertical.handleimage = item.handleimage;
    vertical.handletint = item.handletint;
    vertical.handlelayer = item.handlelayer;
    vertical.handleslice = item.handleslice;
    vertical.handleinset = item.handleinset;
    vertical.size = item.height / item.contentheight;
    vertical.value = item.scrolly / item.contentheight;

    if (item.hoveredy)
      vertical.handletint.a *= 2.0f;

    draw(spritelist, buildstate, vertical);
  }
}
