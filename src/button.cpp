//
// datum-ui - button
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "button.h"
#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

static constexpr float MinDragDistance = 3.0f;
static constexpr float DoubleClickTime = 0.5f;

namespace Ui
{
  //|---------------------- Button --------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Button>(Node *node, DatumPlatform::GameInput const &input, float dt)
  {
    update_item<Control>(node, input, dt);

    auto item = item_cast<Button>(node);

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        dragging = false;
        presspos = item->unmap(mousepos);

        actions.push_back(Action{ item->action, Button::Pressed, ui, item });

        pressitem = item;
      }
    }

    if (focusitem == item)
    {
      elapsed += dt;

      if (hoveritem != item)
        focusitem = nullptr;
    }

    if (pressitem == item)
    {
      if (!input.mousebuttons[GameInput::Left].down())
      {
        actions.push_back(Action{ item->action, Button::Released, ui, item });

        if (hoveritem == item)
        {
          actions.push_back(Action{ item->action, Button::Clicked, ui, item });

          if (focusitem == item && elapsed < DoubleClickTime)
          {
            actions.push_back(Action{ item->action, Button::DoubleClicked, ui, item });
          }

          focusitem = item;
          elapsed = 0;
        }

        pressitem = nullptr;
      }

      inputaccepted = true;
    }

    item->pressed = (pressitem == item) && (hoveritem == item || dragging);
  }

  template<>
  void Context::prepare_item<Button>(Node *node)
  {
    prepare_item<Control>(node);
  }

  template<>
  void Context::render_item<Button>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<Button>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item);

    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- CheckButton ---------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<CheckButton>(Node *node, DatumPlatform::GameInput const &input, float dt)
  {
    auto item = item_cast<CheckButton>(node);

    if (!inputaccepted && hoveritem == item)
    {
      if (input.mousebuttons[GameInput::Left].pressed())
      {
        pressitem = item;
      }
    }

    if (pressitem == item)
    {
      if (!input.mousebuttons[GameInput::Left].down())
      {
        if (hoveritem == item)
        {
          item->checked = (item->checked == CheckButton::Checked) ? CheckButton::UnChecked : CheckButton::Checked;
        }
      }
    }

    update_item<Button>(node, input, dt);
  }

  template<>
  void Context::prepare_item<CheckButton>(Node *node)
  {
    prepare_item<Button>(node);
  }

  template<>
  void Context::render_item<CheckButton>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<CheckButton>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item);

    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- DragButton ----------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<DragButton>(Node *node, DatumPlatform::GameInput const &input, float dt)
  {
    auto item = item_cast<DragButton>(node);

    if (pressitem == item)
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
        item->dx = item->unmap(mousepos).x - presspos.x;
        item->dy = item->unmap(mousepos).y - presspos.y;

        actions.push_back(Action{ item->action, DragButton::Dragged, ui, item });
      }
    }

    update_item<Button>(node, input, dt);
  }

  template<>
  void Context::prepare_item<DragButton>(Node *node)
  {
    prepare_item<Button>(node);

    auto item = item_cast<DragButton>(node);

    if (pressitem == item)
    {
      if (dragging)
      {
        item->dx = item->unmap(mousepos).x - presspos.x;
        item->dy = item->unmap(mousepos).y - presspos.y;
      }
    }
  }

  template<>
  void Context::render_item<DragButton>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<DragButton>(node);

    if (item->visible && item->enabled && item->contains(mousepos) && contains(buildstate.cliprect(), mousepos))
      hoveritem = item;

    draw(spritelist, buildstate, *item);

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_button ///////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Button const &item)
{
  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto padding = item.scale * item.padding;
  auto spacing = item.scale * item.spacing;

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

  Ui::Image image;
  image.x = item.x + padding * xbasis.x + padding * ybasis.x;
  image.y = item.y + padding * xbasis.y + padding * ybasis.y;
  image.width = item.width - 2*padding;
  image.height = item.height - 2*padding;
  image.scale = item.scale;
  image.rotation = item.rotation;
  image.opacity = item.opacity;
  image.image = item.image;
  image.tint = item.imagetint;
  image.layer = item.imagelayer;
  image.scale = item.scale;
  image.fillmode = Ui::Image::Pad;

  switch (item.imagealign)
  {
    case Ui::Button::ImageAlign::Before:
      image.horizontalalign = Ui::Image::Left;
      break;

    case Ui::Button::ImageAlign::After:
      image.horizontalalign = Ui::Image::Right;
      break;

    case Ui::Button::ImageAlign::Above:
      image.verticalalign = Ui::Image::Top;
      break;

    case Ui::Button::ImageAlign::Below:
      image.verticalalign = Ui::Image::Bottom;
      break;

    case Ui::Button::ImageAlign::Fill:
      image.fillmode = Ui::Image::Stretch;
      break;
  }

  draw(spritelist, buildstate, image);

  Ui::Text text;
  text.x = item.x + padding * xbasis.x + padding * ybasis.x;
  text.y = item.y + padding * xbasis.y + padding * ybasis.y;
  text.width = item.width - 2*padding;
  text.height = item.height - 2*padding;
  text.scale = item.scale;
  text.rotation = item.rotation;
  text.opacity = item.opacity;
  text.text = item.text;
  text.font = item.font;
  text.color = item.textcolor;

  if (item.image)
  {
    auto imagewidth = item.scale * item.image->width;
    auto imageheight = item.scale * item.image->height;

    switch (item.imagealign)
    {
      case Ui::Button::ImageAlign::Before:
        text.x += (imagewidth + padding + spacing) * xbasis.x;
        text.y += (imagewidth + padding + spacing) * xbasis.y;
        text.width -= imagewidth + padding + spacing;
        break;

      case Ui::Button::ImageAlign::After:
        text.width -= imagewidth + padding + spacing;
        break;

      case Ui::Button::ImageAlign::Above:
        text.x += (imageheight + padding + spacing) * ybasis.x;
        text.y += (imageheight + padding + spacing) * ybasis.y;
        text.height -= imageheight;
        break;

      case Ui::Button::ImageAlign::Below:
        text.height -= imageheight + padding + spacing;
        break;

      case Ui::Button::ImageAlign::Fill:
        break;
    }
  }

  switch (item.textalign)
  {
    case Ui::Button::TextAlign::Left:
      text.horizontalalign = Ui::Text::HorizontalAlign::Left;
      break;

    case Ui::Button::TextAlign::Center:
      text.horizontalalign = Ui::Text::HorizontalAlign::Center;
      break;

    case Ui::Button::TextAlign::Right:
      text.horizontalalign = Ui::Text::HorizontalAlign::Right;
      break;
  }

  draw(spritelist, buildstate, text);
}


///////////////////////// draw_checkbutton //////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::CheckButton const &item)
{
  auto boxsize = 16;
  auto boxmargin = 2;

  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto padding = item.scale * item.padding;

  if (item.image)
  {
    draw(spritelist, buildstate, static_cast<Ui::Button const &>(item));
  }

  if (!item.image)
  {
    float x = 0, y = 0;

    auto outersize = item.scale * boxsize;
    auto innersize = item.scale * (boxsize - 2*boxmargin);

    switch (item.imagealign)
    {
      case Ui::Button::ImageAlign::Before:
        x = padding;
        y = 0.5f*(item.height - outersize);
        break;

      case Ui::Button::ImageAlign::After:
        x = item.width - outersize - padding;
        y = 0.5f*(item.height - outersize);
        break;

      case Ui::Button::ImageAlign::Above:
        x = 0.5f*(item.width - outersize);
        y = padding;
        break;

      case Ui::Button::ImageAlign::Below:
        x = 0.5f*(item.width - outersize);
        y = item.height - outersize - padding;
        break;
    }

    Ui::Button button;
    button.x = item.x;
    button.y = item.y;
    button.width = item.width;
    button.height = item.height;
    button.scale = item.scale;
    button.rotation = item.rotation;
    button.opacity = item.opacity;
    button.backgroundimage = item.backgroundimage;
    button.backgroundcolor = item.backgroundcolor;
    button.backgroundlayer = item.backgroundlayer;
    button.backgroundslice = item.backgroundslice;
    button.padding = item.padding;

    draw(spritelist, buildstate, button);

    Ui::Rectangle outer;
    outer.x = round(item.x + x*xbasis.x + y*ybasis.x);
    outer.y = round(item.y + x*xbasis.y + y*ybasis.y);
    outer.width = outersize;
    outer.height = outersize;
    outer.rotation = item.rotation;
    outer.opacity = item.opacity;
    outer.color = item.outercolor;
    outer.borderwidth = 1;
    outer.bordercolor = item.textcolor;

    draw(spritelist, buildstate, outer);

    Ui::Rectangle inner;
    inner.x = round(outer.x + item.scale*boxmargin*xbasis.x + item.scale*boxmargin*ybasis.x);
    inner.y = round(outer.y + item.scale*boxmargin*xbasis.y + item.scale*boxmargin*ybasis.y);
    inner.width = innersize;
    inner.height = innersize;
    inner.rotation = item.rotation;
    inner.opacity = item.opacity;
    inner.color = item.innercolor;

    draw(spritelist, buildstate, inner);

    Ui::Button text;
    text.x = item.x;
    text.y = item.y;
    text.width = item.width;
    text.height = item.height;
    text.scale = item.scale;
    text.rotation = item.rotation;
    text.opacity = item.opacity;
    text.text = item.text;
    text.font = item.font;
    text.textcolor = item.textcolor;
    text.textalign = item.textalign;
    text.padding = item.padding;

    switch (item.imagealign)
    {
      case Ui::Button::ImageAlign::Before:
        text.x += (item.textalign == Ui::CheckButton::Left) ? (outersize + padding) * xbasis.x : 0;
        text.y += (item.textalign == Ui::CheckButton::Left) ? (outersize + padding) * xbasis.y : 0;
        text.width -= (item.textalign == Ui::CheckButton::Left) ? outersize + padding : 0;
        break;

      case Ui::Button::ImageAlign::After:
        text.width -= (item.textalign == Ui::CheckButton::Right) ? outersize + padding : 0;
        break;
    }

    draw(spritelist, buildstate, text);
  }
}


///////////////////////// draw_dragbutton ///////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::DragButton const &item)
{
  draw(spritelist, buildstate, static_cast<Ui::Button const &>(item));
}
