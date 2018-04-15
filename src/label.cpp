//
// datum-ui - label
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "label.h"
#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

namespace Ui
{
  //|---------------------- Label ---------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Label>(Node *node, DatumPlatform::GameInput const &input, float dt)
  {
    update_item<Control>(node, input, dt);
  }

  template<>
  void Context::prepare_item<Label>(Node *node)
  {
    prepare_item<Control>(node);
  }

  template<>
  void Context::render_item<Label>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    draw(spritelist, buildstate, *item_cast<Label>(node));

    render_item<Item>(spritelist, buildstate, node);
  }


  //|---------------------- Field ---------------------------------------------
  //|--------------------------------------------------------------------------

  template<>
  void Context::update_item<Field>(Node *node, DatumPlatform::GameInput const &input, float dt)
  {
    update_item<Control>(node, input, dt);
  }

  template<>
  void Context::prepare_item<Field>(Node *node)
  {
    prepare_item<Control>(node);
  }

  template<>
  void Context::render_item<Field>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    draw(spritelist, buildstate, *item_cast<Field>(node));

    render_item<Item>(spritelist, buildstate, node);
  }

}


///////////////////////// draw_label ////////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Label const &item)
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
  text.horizontalalign = item.horizontalalign;
  text.verticalalign = item.verticalalign;

  draw(spritelist, buildstate, text);
}


///////////////////////// draw_field ////////////////////////////////////////
void draw(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Field const &item)
{
  auto xbasis = rotate(Vec2(1, 0), item.rotation);
  auto ybasis = rotate(Vec2(0, 1), item.rotation);
  auto padding = item.scale * item.padding;

  char buffer[128];
  snprintf(buffer, sizeof(buffer), item.format, item.precision, item.value);

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

  Ui::Text text;
  text.x = item.x + padding * xbasis.x + padding * ybasis.x;
  text.y = item.y + padding * xbasis.y + padding * ybasis.y;
  text.width = item.width - 2*padding;
  text.height = item.height - 2*padding;
  text.scale = item.scale;
  text.rotation = item.rotation;
  text.opacity = item.opacity;
  text.text = buffer;
  text.font = item.font;
  text.color = item.textcolor;
  text.horizontalalign = item.horizontalalign;
  text.verticalalign = item.verticalalign;

  draw(spritelist, buildstate, text);
}
