//
// datum-ui - textarea
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "textarea.h"
#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

static constexpr float MinDragDistance = 3.0f;

namespace
{
  int index_left(const char *str, int index)
  {
    index -= 1;

    while ((str[index] & 0xC0) == 0x80)
      --index;

    return index;
  }

  int index_right(const char *str, int index)
  {
    index += 1;

    while ((str[index] & 0xC0) == 0x80)
      ++index;

    return index;
  }

  float line_position(TypeFont const *font, const char *text, int index, int verticalalign, float areaheight, float lineheight)
  {
    float y = 0;

    if (verticalalign != Ui::TextArea::Top)
    {
      int lines = 1;
      for(const char *ch = text; *ch; ++ch)
      {
        if (*ch == '\n')
          ++lines;
      }

      switch (verticalalign)
      {
        case Ui::TextArea::VerticalAlign::Middle:
          y += floor(0.5f * (areaheight - lines * lineheight));
          break;

        case Ui::TextArea::VerticalAlign::Bottom:
          y += (areaheight - lines * lineheight);
          break;
      }
    }

    if (index != 0)
    {
      for(const char *ch = text; true; ++ch)
      {
        auto end = ch;
        while (*end && *end != '\n')
          ++end;

        if (index <= end - text)
          break;

        if (*end == 0)
          break;

        y += lineheight;
        ch = end;
      }
    }

    return y;
  }

  float char_position(TypeFont const *font, const char *beg, const char *end, int index, int horizontalalign, float areawidth, float linewidth, float *extraspace)
  {
    auto x = 0.0f;

    switch (horizontalalign)
    {
      case Ui::TextArea::HorizontalAlign::Left:
        break;

      case Ui::TextArea::HorizontalAlign::Center:
        x += floor(0.5f * (areawidth - linewidth));
        break;

      case Ui::TextArea::HorizontalAlign::Right:
        x += areawidth - linewidth;
        break;

      case Ui::TextArea::HorizontalAlign::Justified:
        break;
    }

    if (horizontalalign == Ui::TextArea::HorizontalAlign::Justified)
    {
      int spaces = 0, totalspaces = 0;
      for(const char *ch = beg; ch != end; ++ch)
      {
        if (*ch == 32 && ch - beg < index)
          ++spaces;

        if (*ch == 32)
          ++totalspaces;
      }

      if (totalspaces != 0)
      {
        *extraspace = (areawidth - linewidth) / totalspaces;
      }

      x += spaces * *extraspace;
    }

    return x + font->width(beg, index);
  }

  Vec2 index_position(TypeFont const *font, const char *str, int index, int horizontalalign, int verticalalign, float areawidth, float areaheight)
  {
    if (!font || !font->ready() || !str)
      return Vec2(0);

    float extraspace = 0.0f;

    auto beg = str + index;
    while (beg > str && *(beg-1) != '\n')
      --beg;

    auto end = str + index;
    while (*end && *end != '\n')
      ++end;

    auto y = line_position(font, str, index, verticalalign, areaheight, font->lineheight());
    auto x = char_position(font, beg, end, str + index - beg, horizontalalign, areawidth, font->width(beg, end - beg), &extraspace);

    return Vec2(x, y);
  }

  int position_index(TypeFont const *font, const char *str, Vec2 const &pos, int horizontalalign, int verticalalign, float areawidth, float areaheight)
  {
    if (!font || !font->ready() || !str)
      return 0;

    auto lineheight = font->lineheight();

    auto x = 0;
    auto y = line_position(font, str, 0, verticalalign, areaheight, lineheight);

    for(const char *ch = str; true; ++ch)
    {
      auto end = ch;
      while (*end && *end != '\n')
        ++end;

      auto min = Vec2(x, y);
      auto max = Vec2(areawidth - x, y + lineheight);

      if (pos.y < max.y || !*end)
      {
        float linewidth = font->width(ch, end - ch);
        float extraspace = 0.0f;

        auto x = pos.x - min.x - char_position(font, ch, end, 0, horizontalalign, max.x - min.x, linewidth, &extraspace);

        auto index = pick_text(font, ch, end - ch, extraspace, x);

        return ch - str + index;
      }

      if (*end == 0)
        break;

      y += lineheight;
      ch = end;
    }

    return 0;
  }
}

namespace Ui
{
  //|---------------------- TextArea ------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// position //////////////////////////////////////////
  int TextArea::position(int index, Position to)
  {
    Vec2 pt;

    switch (to)
    {
      case Start:
        return 0;

      case CharLeft:
        if (index > 0)
          index = index_left(text, index);
        return index;

      case CharRight:
        if (text[index])
          index = index_right(text, index);
        return index;

      case WordLeft:
        if (index > 0 && isalnum(text[index-1]))
          while (index > 0 && !isspace(text[index-1]))
            index = index_left(text, index);
        else
          while (index > 0 && !isalnum(text[index-1]))
            index = index_left(text, index);
        return index;

      case WordRight:
        if (text[index] && isalnum(text[index]))
          while (text[index] && !isspace(text[index]))
            index = index_right(text, index);
        else
          while (text[index] && !isalnum(text[index]))
            index = index_right(text, index);
        return index;

      case LineUp:
        pt = index_position(font, text, index, horizontalalign, verticalalign, width, height);
        return position_index(font, text, Vec2(pt.x, pt.y - font->lineheight()), horizontalalign, verticalalign, width, height);

      case LineDown:
        pt = index_position(font, text, index, horizontalalign, verticalalign, width, height);
        return position_index(font, text, Vec2(pt.x, pt.y + font->lineheight()), horizontalalign, verticalalign, width, height);

      case LineStart:
        while (index > 0 && text[index-1] != '\n')
          index = index_left(text, index);
        return index;

      case LineEnd:
        while (text[index] && text[index] != '\n')
          index = index_right(text, index);
        return index;

      case End:
        return index + strlen(text + index);
    }

    throw runtime_error("Bad Position");
  }


  ///////////////////////// position //////////////////////////////////////////
  int TextArea::position(Vec2 const &worldpos)
  {
    return position_index(font, text, unmap(worldpos), horizontalalign, verticalalign, width, height);
  }


  ///////////////////////// position //////////////////////////////////////////
  Vec2 TextArea::position(int index)
  {
    return map(index_position(font, text, index, horizontalalign, verticalalign, width, height));
  }


  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<TextArea>(Node *node, GameInput const &input, float dt)
  {
    auto item = item_cast<TextArea>(node);

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        dragging = false;
        presspos = mousepos;

        if (item->interactive)
        {
          item->selectionbeg = item->selectionend = item->cursor = item->position(presspos);

          focusitem = item;
        }

        pressitem = item;
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
          if (evt.key == 'A' && (evt.modifiers & GameInput::Modifiers::Control))
          {
            item->selectionbeg = 0;
            item->selectionend = item->cursor = item->position(item->cursor, TextArea::End);
          }

          if (evt.key == KB_KEY_LEFT || evt.key == KB_KEY_RIGHT || evt.key == KB_KEY_UP || evt.key == KB_KEY_DOWN || evt.key == KB_KEY_HOME || evt.key == KB_KEY_END)
          {
            switch (evt.key)
            {
              case KB_KEY_LEFT:
                item->cursor = item->position(item->cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::WordLeft : TextArea::CharLeft);
                break;

              case KB_KEY_RIGHT:
                item->cursor = item->position(item->cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::WordRight : TextArea::CharRight);
                break;

              case KB_KEY_UP:
                item->cursor = item->position(item->cursor, TextArea::LineUp);
                break;

              case KB_KEY_DOWN:
                item->cursor = item->position(item->cursor, TextArea::LineDown);
                break;

              case KB_KEY_HOME:
                item->cursor = item->position(item->cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::Start : TextArea::LineStart);
                break;

              case KB_KEY_END:
                item->cursor = item->position(item->cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::End : TextArea::LineEnd);
                break;
            }

            if (!(evt.modifiers & GameInput::Modifiers::Shift))
              item->selectionbeg = item->cursor;

            item->selectionend = item->cursor;
          }
        }
      }

      if (input.mousebuttons[GameInput::Left].pressed() && pressitem != item)
        focusitem = nullptr;
    }

    if (pressitem == item)
    {
      if (!dragging)
      {
        if (dist(presspos, mousepos) > MinDragDistance)
        {
          dragging = true;
        }
      }

      if (dragging)
      {
        item->selectionend = item->cursor = item->position(mousepos);
      }

      if (!input.mousebuttons[GameInput::Left].down())
      {
        pressitem = nullptr;
      }
    }

    if (focusitem != item)
      item->selectionbeg = item->selectionend = item->cursor = -1;
  }


  ///////////////////////// request_item //////////////////////////////////////
  template<>
  void Context::request_item<TextArea>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    auto item = item_cast<TextArea>(node);

    request(platform, fontcatalog, item->font, ready, total);
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<TextArea>(Node *node)
  {
    prepare_item<Item>(node);
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<TextArea>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<TextArea>(node);

    if (item->visible && item->interactive && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item_cast<TextArea>(node));

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_textarea /////////////////////////////////////

void draw_line(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::TextArea const &item, Rect2 const &rect, const char *beg, const char *end)
{
  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);

  auto origin = Vec2(item.x, item.y) + rect.min.x * xbasis + rect.min.y * ybasis;

  float linewidth = item.font->width(beg, end - beg);
  float lineheight = item.font->lineheight();
  float extraspace = 0.0f;

  auto cursor = origin + item.font->ascent * item.font->pixelscale * ybasis;

  auto textcolor = item.textcolor * item.opacity;
  auto selectioncolor = item.selectioncolor * item.opacity;
  auto selectiontextcolor = item.selectiontextcolor * item.opacity;

  auto selectionbeg = item.text + min(item.selectionbeg, item.selectionend);
  auto selectionend = item.text + max(item.selectionbeg, item.selectionend);

  if (selectionbeg < selectionend && beg < selectionend && selectionbeg < end)
  {
    auto x0 = char_position(item.font, beg, end, 0, item.horizontalalign, rect.max.x - rect.min.x, linewidth, &extraspace);
    auto x1 = char_position(item.font, beg, end, max(beg, selectionbeg) - beg, item.horizontalalign, rect.max.x - rect.min.x, linewidth, &extraspace);
    auto x2 = char_position(item.font, beg, end, min(end, selectionend) - beg, item.horizontalalign, rect.max.x - rect.min.x, linewidth, &extraspace);

    if (beg < selectionbeg)
    {
      draw_text(spritelist, buildstate, cursor + x0*xbasis, xbasis, ybasis, item.font, beg, selectionbeg - beg, extraspace, textcolor);
    }

    if (selectionend < end)
    {
      draw_text(spritelist, buildstate, cursor + x2*xbasis, xbasis, ybasis, item.font, selectionend, end - selectionend, extraspace, textcolor);
    }

    if (x1 != x2)
    {
      spritelist.push_rect(buildstate, origin, Rect2(Vec2(x1, 0), Vec2(x2, lineheight)), item.rotation, selectioncolor);

      draw_text(spritelist, buildstate, cursor + x1*xbasis, xbasis, ybasis, item.font, max(beg, selectionbeg), min(end, selectionend) - max(beg, selectionbeg), extraspace, selectiontextcolor);
    }
  }
  else
  {
    auto x = char_position(item.font, beg, end, 0, item.horizontalalign, rect.max.x - rect.min.x, linewidth, &extraspace);

    draw_text(spritelist, buildstate, cursor + x*xbasis, xbasis, ybasis, item.font, beg, end - beg, extraspace, textcolor);
  }

  if (item.cursor != -1)
  {
    auto cursorloc = item.text + item.cursor;

    if (beg <= cursorloc && cursorloc <= end)
    {
      auto x = char_position(item.font, beg, end, cursorloc - beg, item.horizontalalign, rect.max.x - rect.min.x, linewidth, &extraspace);

      spritelist.push_rect(buildstate, origin, Rect2(Vec2(x-1, 0), Vec2(x+1, lineheight)), item.rotation, textcolor);
    }
  }
}

void draw_area(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::TextArea const &item)
{
  auto lineheight = item.font->lineheight();

  auto x = 0;
  auto y = line_position(item.font, item.text, 0, item.verticalalign, item.height, lineheight);

  for(const char *ch = item.text; true; ++ch)
  {
    auto end = ch;
    while (*end && *end != '\n')
      ++end;

    auto min = Vec2(x, y);
    auto max = Vec2(item.width - x, y + lineheight);

    if (intersects(Rect2(item.pos() + min, item.pos() + max), buildstate.cliprect()))
    {
      draw_line(spritelist, buildstate, item, Rect2(min, max), ch, end);
    }

    if (*end == 0)
      break;

    y += lineheight;
    ch = end;
  }
}

void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::TextArea const &item)
{
  if (item.opacity != 0 && item.width != 0 && item.height != 0 && item.text != nullptr && item.font != nullptr && item.font->ready())
  {
    draw_area(spritelist, buildstate, item);
  }
}
