//
// datum-ui - combobox
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "combobox.h"
#include "context.h"
#include "button.h"
#include "listbox.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace leap;
using namespace DatumPlatform;

namespace Ui
{
  struct ComboBoxDropList : ListBox
  {
    ComboBox *combobox;
  };

  //|---------------------- ComboBoxDropList ----------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<ComboBoxDropList>(Node *node, GameInput const &input, float dt)
  {
    update_item<ListBox>(node, input, dt);

    auto item = item_cast<ComboBoxDropList>(node);

    if (hoveritem == item)
    {
      if (mousepos.x < item->x + item->width - item->scale * item->handlesize || item->contentheight <= item->height)
      {
        float lineheight = item->font ? item->font->lineheight() : 0.0f;

        item->selection = min(max(0, (int)((item->unmap(mousepos).y + item->scrolly) / lineheight)), (int)(item->contentheight / lineheight) - 1);
      }
    }
    else
    {
      item->selection = -1;
    }

    if (focusitem == item)
    {
      if (input.keys[KB_KEY_ESCAPE].pressed())
      {
        focusitem = nullptr;
      }

      for(auto &action : actions)
      {
        if (action.item == item && action.op == ListBox::Selected)
        {
          item->combobox->selection = item->selection;

          actions.push_back(Action{ item->combobox->action, ComboBox::Changed, ui, item->combobox });

          focusitem = nullptr;
        }
      }

      inputaccepted = true;
    }

    if (focusitem != item)
      destroy(item);
  }


  ///////////////////////// request_item //////////////////////////////////////
  template<>
  void Context::request_item<ComboBoxDropList>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<ComboBoxDropList>(Node *node)
  {
    prepare_item<ListBox>(node);
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<ComboBoxDropList>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<ComboBoxDropList>(node);

    if (item->x < buildstate.cliprect().min.x)
    {
      item->x = buildstate.cliprect().min.x;
    }

    if (item->x + item->width > buildstate.cliprect().max.x)
    {
      item->x = max(buildstate.cliprect().max.x - item->width, 0.0f);
      item->width = buildstate.cliprect().max.x - item->x;
    }

    if (item->y < buildstate.cliprect().min.y)
    {
      item->y = buildstate.cliprect().min.y;
    }

    if (item->y + item->height > buildstate.cliprect().max.y)
    {
      item->y = max(buildstate.cliprect().max.y - item->height, 0.0f);
      item->height = buildstate.cliprect().max.y - item->y;
    }

    render_item<ListBox>(spritelist, buildstate, node);
  }


  //|---------------------- ComboBox ------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<ComboBox>(Node *node, GameInput const &input, float dt)
  {
    update_item<Control>(node, input, dt);

    auto item = item_cast<ComboBox>(node);

    if (!inputaccepted && hoveritem == item)
    {
      if (item->dropped)
        inputaccepted = true;
    }

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        dragging = false;
        presspos = item->unmap(mousepos);

        actions.push_back(Action{ item->action, ComboBox::Pressed, ui, item });

        pressitem = item;
        inputaccepted = true;
      }

      if (input.deltamousez != 0)
      {
        if (item->font && item->text)
        {
          int lines = 1;
          for(const char *ch = item->text; *ch; ++ch)
          {
            if (*ch == '\n')
              ++lines;
          }

          item->selection = min(max(0, item->selection - (int)sign(input.deltamousez)), lines - 1);

          actions.push_back(Action{ item->action, ComboBox::Changed, ui, item });
        }

        inputaccepted = true;
      }
    }

    if (pressitem == item)
    {
      if (!input.mousebuttons[GameInput::Left].down())
      {
        actions.push_back(Action{ item->action, ComboBox::Released, ui, item });

        if (hoveritem == item)
        {
          auto popup = create_item<Ui::ComboBoxDropList>();

          popup->x = item->x;
          popup->y = item->y + item->height;
          popup->z = 9999;
          popup->width = item->width;
          popup->height = item->height;
          popup->scale = item->scale;
          popup->borderwidth = item->listborderwidth;
          popup->bordercolor = item->listbordercolor;
          popup->backgroundcolor = item->listbackgroundcolor;
          popup->font = item->font;
          popup->text = item->text;
          popup->textcolor = item->textcolor;
          popup->selectioncolor = item->selectioncolor;
          popup->selectiontextcolor = item->selectiontextcolor;
          popup->handleimage = item->handleimage;
          popup->handletint = item->handletint;
          popup->handlelayer = item->handlelayer;
          popup->handleslice = item->handleslice;
          popup->handlesize = item->handlesize;
          popup->handleinset = item->handleinset;
          popup->combobox = item;

          if (item->font && item->text)
          {
            int lines = 1;
            for(const char *ch = item->text; *ch; ++ch)
            {
              if (*ch == '\n')
                ++lines;
            }

            popup->height = ceil(lines * item->font->lineheight());
          }

          popupitem = popup;
          popupowner = item;
          focusitem = popup;
        }

        pressitem = nullptr;
      }
    }

    item->pressed = (pressitem == item) && (hoveritem == item || dragging);
    item->dropped = (popupowner == item);
  }


  ///////////////////////// request_item //////////////////////////////////////
  template<>
  void Context::request_item<ComboBox>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
    auto item = item_cast<ComboBox>(node);

    request(platform, spritecatalog, item->dropimage, ready, total);
    request(platform, spritecatalog, item->handleimage, ready, total);

    request_item<Control>(platform, node, ready, total);
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<ComboBox>(Node *node)
  {
    prepare_item<Control>(node);
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<ComboBox>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<ComboBox>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item_cast<ComboBox>(node));

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_combobox /////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::ComboBox const &item)
{
  char buttontext[256] = "";

  if (item.text)
  {
    auto ch = item.text;

    for(int line = 0; line < item.selection && *ch; ++ch)
    {
      if (*ch == '\n')
        ++line;
    }

    auto end = ch;
    while (*end && *end != '\n')
      ++end;

    strlcpy(buttontext, ch, min(size_t(end - ch + 1), sizeof(buttontext)));
  }

  Ui::Button button;
  button.x = item.x;
  button.y = item.y;
  button.width = item.width;
  button.height = item.height;
  button.scale = item.scale;
  button.rotation = item.rotation;
  button.opacity = item.opacity;
  button.borderwidth = item.borderwidth;
  button.bordercolor = item.bordercolor;
  button.backgroundimage = item.backgroundimage;
  button.backgroundcolor = item.backgroundcolor;
  button.backgroundlayer = item.backgroundlayer;
  button.backgroundslice = item.backgroundslice;
  button.font = item.font;
  button.fontheight = item.fontheight;
  button.text = buttontext;
  button.textcolor = item.textcolor;
  button.textalign = Ui::Button::Left;
  button.padding = item.padding;
  button.image = item.dropimage;
  button.imagetint = item.dropimagetint;
  button.imagelayer = item.dropimagelayer;
  button.imagealign = Ui::Button::After;

  draw(spritelist, buildstate, button);
}
