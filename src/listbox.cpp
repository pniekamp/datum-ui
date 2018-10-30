//
// datum-ui - listbox
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "listbox.h"
#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

namespace Ui
{
  //|---------------------- ListBox -------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<ListBox>(Node *node, GameInput const &input, float dt)
  {
    update_item<Control>(node, input, dt);

    auto item = item_cast<ListBox>(node);

    float lineheight = item->font ? item->font->lineheight() : 0.0f;

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        dragging = false;
        presspos = item->unmap(mousepos);

        actions.push_back(Action{ item->action, ListBox::Pressed, ui, item });

        focusitem = item;
        pressitem = item;
        inputaccepted = true;
      }

      if (input.deltamousez != 0)
      {
        item->scrolly -= input.deltamousez * lineheight;

        inputaccepted = true;
      }
    }

    if (focusitem == item)
    {
      for(auto i = 0; i < input.eventcount; ++i)
      {
        auto &evt = input.events[i];

        if (evt.type == GameInput::Event::Key)
        {
          switch (evt.key)
          {
            case KB_KEY_UP:
              item->selection = min(max(0, item->selection - 1), (int)(item->contentheight / lineheight) - 1);
              break;

            case KB_KEY_DOWN:
              item->selection = min(item->selection + 1, (int)(item->contentheight / lineheight) - 1);
              break;

            case KB_KEY_HOME:
              item->selection = min(0, (int)(item->contentheight / lineheight) - 1);
              break;

            case KB_KEY_END:
              item->selection = (int)(item->contentheight / lineheight) - 1;
              break;
          }
        }
      }

      if (item->selection != -1)
      {
        auto cursory = item->selection * lineheight - item->scrolly;

        if (cursory < 0)
        {
          item->scrolly -= 0.0f - cursory;
        }

        if (cursory > item->height - lineheight)
        {
          item->scrolly += cursory - item->height + lineheight;
        }
      }

      if (input.mousebuttons[GameInput::Left].pressed() && pressitem != item)
        focusitem = nullptr;
    }

    if (pressitem == item)
    {
      bool handled = false;

      if (item->height < item->contentheight)
      {
        if (!handled && presspos.x > item->width - item->scale * item->handlesize && presspos.x < item->width && presspos.y > 0 && presspos.y < item->height)
        {
          item->scrolly = round((item->unmap(mousepos).y - 0.5f * item->height / item->contentheight * item->height) / item->height * item->contentheight);

          handled = true;
        }
      }

      if (!handled && presspos.x > 0 && presspos.x < item->width && presspos.y > 0 && presspos.y < item->height)
      {
        item->selection = min(max(0, (int)((item->unmap(mousepos).y + item->scrolly) / lineheight)), (int)(item->contentheight / lineheight) - 1);

        if (item->selection != -1)
        {
          auto cursory = item->selection * lineheight - item->scrolly;

          if (cursory < 0)
          {
            item->scrolly -= 0.0f - cursory;
          }

          if (cursory > item->height - lineheight)
          {
            item->scrolly += cursory - item->height + lineheight;
          }
        }

        if (!input.mousebuttons[GameInput::Left].down())
        {
          actions.push_back(Action{ item->action, ListBox::Selected, ui, item });
        }

        handled = true;
      }

      if (!input.mousebuttons[GameInput::Left].down())
      {
        actions.push_back(Action{ item->action, ListBox::Released, ui, item });

        pressitem = nullptr;
      }
    }

    item->scrolly = clamp(item->scrolly, 0.0f, item->contentheight - item->height);

    item->focused = (focusitem == item);
  }


  ///////////////////////// request_item //////////////////////////////////////
  template<>
  void Context::request_item<ListBox>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    auto item = item_cast<ListBox>(node);

    request(platform, spritecatalog, item->handleimage, ready, total);

    request_item<Control>(platform, node, ready, total);
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<ListBox>(Node *node)
  {
    prepare_item<Control>(node);

    auto item = item_cast<ListBox>(node);

    if (item->font && item->text)
    {
      int lines = 1;
      for(const char *ch = item->text; *ch; ++ch)
      {
        if (*ch == '\n')
          ++lines;
      }

      item->contentheight = lines * item->font->lineheight();
    }

    item->scrolly = clamp(item->scrolly, 0.0f, item->contentheight - item->height);
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<ListBox>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<ListBox>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item_cast<ListBox>(node));

    render_item<Item>(spritelist, buildstate, node);

    auto xbasis = rotate(Vec2(1, 0), item->rotation);

    if (item->height < item->contentheight)
    {
      Ui::ScrollBar vertical;
      vertical.x = item->x + (item->width - item->scale * item->handlesize)*xbasis.x;
      vertical.y = item->y + (item->width - item->scale * item->handlesize)*xbasis.y;
      vertical.width = item->scale * item->handlesize;
      vertical.height = item->height;
      vertical.scale = item->scale;
      vertical.rotation = item->rotation;
      vertical.opacity = item->opacity;
      vertical.orientation = Ui::ScrollBar::Vertical;
      vertical.handleimage = item->handleimage;
      vertical.handletint = item->handletint;
      vertical.handlelayer = item->handlelayer;
      vertical.handleslice = item->handleslice;
      vertical.handleinset = item->handleinset;
      vertical.size = item->height / item->contentheight;
      vertical.value = item->scrolly / item->contentheight;

      draw(spritelist, buildstate, vertical);
    }
  }
}


///////////////////////// draw_listbox //////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ListBox const &item)
{
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

  draw(spritelist, buildstate, frame);

  if (item.opacity != 0 && item.width != 0 && item.height != 0 && item.text != nullptr && item.font != nullptr && item.font->ready())
  {
    auto cliprect = buildstate.cliprect();
    auto arearect = Rect2(Vec2(item.x, item.y), Vec2(item.x + item.width - 1, item.y + item.height - 1));

    if (auto newclip = intersection(cliprect, arearect))
    {
      spritelist.push_scissor(buildstate, *newclip);

      auto xbasis = rotate(Vec2(1, 0), item.rotation);
      auto ybasis = rotate(Vec2(0, 1), item.rotation);
      auto padding = item.scale * item.padding;

      float lineheight = item.font->lineheight();
      float extraspace = 0.0f;

      auto textcolor = item.textcolor * item.opacity;
      auto selectioncolor = item.selectioncolor * item.opacity;
      auto selectiontextcolor = item.selectiontextcolor * item.opacity;

      auto line = 0;
      auto x = padding;
      auto y = 0.0f - item.scrolly;

      for(const char *ch = item.text; *ch; ++line)
      {
        auto end = ch;
        while (*end && *end != '\n')
          ++end;

        auto min = Vec2(x, y);
        auto max = Vec2(item.width - x, y + lineheight);

        if (intersects(Rect2(item.pos() + min, item.pos() + max), buildstate.cliprect()))
        {
          auto cursor = Vec2(item.x, item.y) + x * xbasis + (y + item.font->ascent * item.font->pixelscale) * ybasis;

          if (line == item.selection)
          {
            spritelist.push_rect(buildstate, Vec2(item.x, item.y), Rect2(Vec2(0, y), Vec2(item.width, y + lineheight)), item.rotation, selectioncolor);

            draw_text(spritelist, buildstate, cursor, xbasis, ybasis, item.font, ch, end - ch, extraspace, selectiontextcolor);
          }
          else
          {
            draw_text(spritelist, buildstate, cursor, xbasis, ybasis, item.font, ch, end - ch, extraspace, textcolor);
          }
        }

        y += lineheight;
        ch = *end ? end + 1: end;
      }

      spritelist.push_scissor(buildstate, cliprect);
    }
  }
}
