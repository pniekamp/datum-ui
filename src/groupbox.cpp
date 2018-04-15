//
// datum-ui - groupbox
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "groupbox.h"
#include "context.h"
#include "button.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace leap;
using namespace DatumPlatform;

namespace Ui
{
  //|---------------------- GroupBox ------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<GroupBox>(Node *node, GameInput const &input, float dt)
  {
    update_item<CheckButton>(node, input, dt);
  }


  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<GroupBox>(Node *node)
  {
    prepare_item<CheckButton>(node);

    auto item = item_cast<GroupBox>(node);

    auto padding = item->scale * item->padding;
    auto headersize = item->scale * (item->image ? item->image->height : 16) + 2*padding;

    float dx = padding / item->scale;
    float dy = headersize / item->scale;

    for(auto child = node->firstchild; child; child = child->nextsibling)
    {
      auto childitem = item_cast<Ui::Item>(child);

      childitem->x += dx;
      childitem->y += dy;
    }
  }


  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<GroupBox>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    auto item = item_cast<GroupBox>(node);

    if (item->visible && item->enabled && contains(buildstate.cliprect(), mousepos))
    {
      auto mouse = item->unmap(mousepos);

      auto padding = item->scale * item->padding;
      auto headersize = item->scale * (item->image ? item->image->height : 16) + 2*padding;

      if (mouse.x > 0 && mouse.x < item->width && mouse.y > 0 && mouse.y < headersize)
        hoveritem = item;
    }

    draw(spritelist, buildstate, *item_cast<GroupBox>(node));

    render_item<Item>(spritelist, buildstate, node);
  }
}


///////////////////////// draw_groupbox /////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::GroupBox const &item)
{
  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto padding = item.scale * item.padding;

  auto headersize = item.scale * (item.image ? item.image->height : 16) + 2*padding;

  Ui::Rectangle background;
  background.x = item.x + 0.5f*headersize * ybasis.x;
  background.y = item.y + 0.5f*headersize * ybasis.y;
  background.width = item.width;
  background.height = item.height - 0.5f*headersize;
  background.scale = item.scale;
  background.rotation = item.rotation;
  background.opacity = item.opacity;
  background.color = item.backgroundcolor;
  background.borderwidth = item.borderwidth;
  background.bordercolor = item.bordercolor;

  draw(spritelist, buildstate, background);

  Ui::Frame frame;
  frame.x = item.x + 0.5f*headersize * ybasis.x;
  frame.y = item.y + 0.5f*headersize * ybasis.y;
  frame.width = item.width;
  frame.height = item.height - 0.5f*headersize;
  frame.scale = item.scale;
  frame.rotation = item.rotation;
  frame.opacity = item.opacity;
  frame.image = item.backgroundimage;
  frame.layer = item.backgroundlayer;
  frame.slice = item.backgroundslice;

  draw(spritelist, buildstate, frame);

  Ui::CheckButton button;
  button.x = item.x + padding * xbasis.x;
  button.y = item.y + padding * xbasis.y;
  button.width = item.width - 2*padding;
  button.height = headersize;
  button.scale = item.scale;
  button.rotation = item.rotation;
  button.opacity = item.opacity;
  button.backgroundcolor = item.backgroundcolor;
  button.font = item.font;
  button.fontheight = item.fontheight;
  button.text = item.text;
  button.textcolor = item.textcolor;
  button.textalign = item.textalign;
  button.spacing = item.spacing;
  button.image = item.image;
  button.imagetint = item.imagetint;
  button.imagelayer = item.imagelayer;
  button.imagealign = item.imagealign;
  button.checked = item.checked;
  button.innercolor = item.innercolor;
  button.outercolor = item.outercolor;

  draw(spritelist, buildstate, button);
}
