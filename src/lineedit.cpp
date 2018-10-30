//
// datum-ui - lineedit
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "lineedit.h"
#include "context.h"
#include "textarea.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

static constexpr float MinDragDistance = 3.0f;

namespace Ui
{
  //|---------------------- LineEdit ------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<LineEdit>(Node *node, GameInput const &input, float dt)
  {
    update_item<Control>(node, input, dt);

    auto item = item_cast<LineEdit>(node);

    auto xbasis = rotate(Vec2(1, 0), item->rotation);
    auto ybasis = rotate(Vec2(0, 1), item->rotation);
    auto padding = item->scale * item->padding;

    TextArea area;
    area.x = item->x + padding * xbasis.x + padding * ybasis.x - item->scrollx;
    area.y = item->y + padding * xbasis.y + padding * ybasis.y - item->scrolly;
    area.width = item->width - 2*padding + item->scrollx;
    area.height = item->height - 2*padding + item->scrolly;
    area.scale = item->scale;
    area.rotation = item->rotation;
    area.opacity = item->opacity;
    area.font = item->font;
    area.text = item->text;

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        dragging = false;
        presspos = mousepos;

        item->selectionbeg = item->selectionend = item->cursor = area.position(presspos);

        actions.push_back(Action{ item->action, LineEdit::Focused, ui, item });

        focusitem = item;
        pressitem = item;        
        inputaccepted = true;
      }

      cursor = Cursor::Edit;
    }

    if (focusitem == item)
    {
      if (node->strings.empty())
        node->strings.resize(1);

      auto &cursor = item->cursor;
      auto &selectionbeg = item->selectionbeg;
      auto &selectionend = item->selectionend;
      auto &text = node->strings[0];

      if (item->text != text.c_str())
      {
        text = (item->text) ? item->text : "";
      }

      cursor = clamp(cursor, 0, (int)text.size());

      for(auto i = 0; i < input.eventcount; ++i)
      {
        auto &evt = input.events[i];

        if (evt.type == GameInput::Event::Key)
        {
          if (evt.key == 'A' && (evt.modifiers & GameInput::Modifiers::Control))
          {
            selectionbeg = 0;
            selectionend = cursor = area.position(cursor, TextArea::End);
          }

          if (evt.key == KB_KEY_LEFT || evt.key == KB_KEY_RIGHT || evt.key == KB_KEY_HOME || evt.key == KB_KEY_END)
          {
            switch (evt.key)
            {
              case KB_KEY_LEFT:
                cursor = area.position(cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::WordLeft : TextArea::CharLeft);
                break;

              case KB_KEY_RIGHT:
                cursor = area.position(cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::WordRight : TextArea::CharRight);
                break;

              case KB_KEY_HOME:
                cursor = area.position(cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::Start : TextArea::LineStart);
                break;

              case KB_KEY_END:
                cursor = area.position(cursor, (evt.modifiers & GameInput::Modifiers::Control) ? TextArea::End : TextArea::LineEnd);
                break;
            }

            if (!(evt.modifiers & GameInput::Modifiers::Shift))
              selectionbeg = cursor;

            selectionend = cursor;
          }

          if (evt.key == KB_KEY_DELETE && item->filter != Ui::LineEdit::Readonly)
          {
            auto beg = cursor;
            auto end = area.position(cursor, TextArea::CharRight);

            if (selectionbeg != selectionend)
            {
              beg = min(selectionbeg, selectionend);
              end = max(selectionbeg, selectionend);
            }

            text.erase(beg, end - beg);

            selectionbeg = selectionend = cursor = beg;

            actions.push_back(Action{ item->action, LineEdit::Changed, ui, item });
          }

          if (evt.key == KB_KEY_BACKSPACE && item->filter != Ui::LineEdit::Readonly)
          {
            auto beg = area.position(cursor, TextArea::CharLeft);
            auto end = cursor;

            if (selectionbeg != selectionend)
            {
              beg = min(selectionbeg, selectionend);
              end = max(selectionbeg, selectionend);
            }

            text.erase(beg, end - beg);

            selectionbeg = selectionend = cursor = beg;

            actions.push_back(Action{ item->action, LineEdit::Changed, ui, item });
          }
        }

        if (evt.type == GameInput::Event::Text)
        {
          int len = 0;
          char intputtext[128];
          for(auto ch = evt.text; *ch; ++ch)
          {
            bool accept = false;

            if (item->filter & Ui::LineEdit::Alpha)
              accept |= (isalpha(*ch) != 0);

            if (item->filter & Ui::LineEdit::Digit)
              accept |= (isdigit(*ch) != 0);

            if (item->filter & Ui::LineEdit::Integer)
              accept |= (isdigit(*ch) != 0 || *ch == '-' || *ch == '+');

            if (item->filter & Ui::LineEdit::Decimal)
              accept |= (isdigit(*ch) != 0 || *ch == '-' || *ch == '+' || *ch == '.' || *ch == 'e' || *ch == 'E');

            if (item->filter & Ui::LineEdit::Punct)
              accept |= (ispunct(*ch) != 0);

            if (item->filter & Ui::LineEdit::Space)
              accept |= (*ch == ' ');

            if (item->filter & Ui::LineEdit::Extended)
              accept |= ((*ch & 0x80) != 0);

            if (accept)
              intputtext[len++] = *ch;
          }

          if (len != 0)
          {
            if (selectionbeg != selectionend)
            {
              auto beg = min(selectionbeg, selectionend);
              auto end = max(selectionbeg, selectionend);

              text.erase(beg, end - beg);

              cursor = beg;
            }

            text.insert(cursor, intputtext, len);

            selectionbeg = selectionend = cursor = cursor + len;

            actions.push_back(Action{ item->action, LineEdit::Changed, ui, item });
          }
        }
      }

      item->text = text.c_str();

      auto cursorpos = area.position(cursor);

      if (cursorpos.x < item->x + padding)
      {
        item->scrollx -= (item->x + padding) - cursorpos.x;
      }

      if (cursorpos.x > item->x + item->width - padding)
      {
        item->scrollx += cursorpos.x - (item->x + item->width - padding);
      }

      if (input.keys[KB_KEY_UP].pressed())
      {
        actions.push_back(Action{ item->action, LineEdit::Uped, ui, item });
      }

      if (input.keys[KB_KEY_DOWN].pressed())
      {
        actions.push_back(Action{ item->action, LineEdit::Downed, ui, item });
      }

      if (input.keys[KB_KEY_TAB].pressed())
      {
        actions.push_back(Action{ item->action, LineEdit::Tabbed, ui, item });

        focusitem = nullptr;
      }

      if (input.keys[KB_KEY_ESCAPE].pressed())
      {
        actions.push_back(Action{ item->action, LineEdit::Canceled, ui, item });

        focusitem = nullptr;
      }

      if (input.keys[KB_KEY_ENTER].pressed())
      {
        actions.push_back(Action{ item->action, LineEdit::Accepted, ui, item });

        focusitem = nullptr;
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
        item->selectionend = item->cursor = area.position(mousepos);
      }

      if (!input.mousebuttons[GameInput::Left].down())
      {
        pressitem = nullptr;
      }

      cursor = Cursor::Edit;
    }

    if (focusitem != item)
      item->selectionbeg = item->selectionend = -1;

    item->focused = (focusitem == item);
  }


  ///////////////////////// request_item //////////////////////////////////////
  template<>
  void Context::request_item<LineEdit>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    request_item<Control>(platform, node, ready, total);
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<LineEdit>(Node *node)
  {
    prepare_item<Control>(node);
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<LineEdit>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<LineEdit>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item_cast<LineEdit>(node));

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_lineedit /////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::LineEdit const &item)
{
  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto padding = item.scale * item.padding;

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

  Ui::TextArea text;
  text.x = item.x + padding * xbasis.x + padding * ybasis.x - item.scrollx;
  text.y = item.y + padding * xbasis.y + padding * ybasis.y - item.scrolly;
  text.width = item.width - 2*padding + item.scrollx;
  text.height = item.height - 2*padding + item.scrolly;
  text.scale = item.scale;
  text.rotation = item.rotation;
  text.opacity = item.opacity;
  text.font = item.font;
  text.text = item.text;
  text.textcolor = item.textcolor;
  text.cursor = item.focused ? item.cursor : -1;
  text.selectionbeg = item.selectionbeg;
  text.selectionend = item.selectionend;
  text.selectioncolor = item.selectioncolor;
  text.selectiontextcolor = item.selectiontextcolor;

  auto cliprect = buildstate.cliprect();
  auto arearect = Rect2(Vec2(item.x + padding - 1, item.y + padding), Vec2(item.x + item.width - padding + 1, item.y + item.height - padding));

  if (auto newclip = intersection(cliprect, arearect))
  {
    spritelist.push_scissor(buildstate, *newclip);

    draw(spritelist, buildstate, text);

    spritelist.push_scissor(buildstate, cliprect);
  }
}
