//
// datum-ui - context
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

namespace Ui
{
  //|---------------------- Item ----------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<Item>(Node *node, GameInput const &input, float dt)
  {
  }

  ///////////////////////// request_item //////////////////////////////////////
  template<>
  void Context::request_item<Item>(PlatformInterface &platform, Node *node, int *ready, int *total)
  {
  }

  ///////////////////////// prepare_item ///////////////////////////////////////
  template<>
  void Context::prepare_item<Item>(Node *node)
  {
    auto item = item_cast<Item>(node);

    if (node->parent)
    {
      auto parent = item_cast<Item>(node->parent);

      auto position = parent->map(Vec2(item->x, item->y));

      item->x = position.x;
      item->y = position.y;

      item->scale = parent->scale * item->scale;
      item->rotation = parent->rotation + item->rotation;
      item->opacity = parent->opacity * item->opacity;

      item->width = item->scale * item->width;
      item->height = item->scale * item->height;
    }
  }

  ///////////////////////// render_item ///////////////////////////////////////
  template<>
  void Context::render_item<Item>(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    for(auto child = node->firstchild; child; child = child->nextsibling)
    {
      render(spritelist, buildstate, child);
    }
  }


  //|---------------------- Context -------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// Context::Constructor //////////////////////////////
  Context::Context(ResourceManager &resources, allocator_type const &allocator)
    : fontcatalog(resources, allocator),
      spritecatalog(resources, allocator),
      actions(allocator),
      m_allocator(allocator, m_freelist)
  {
    inputaccepted = false;
    mousepos = Vec2(0, 0);
    presspos = Vec2(0, 0);
    hoveritem = nullptr;
    pressitem = nullptr;
    focusitem = nullptr;
    activeitem = nullptr;    
    popupitem = nullptr;
    popupowner = nullptr;
    elapsed = 0.0f;
    timeditem = nullptr;
  }


  ///////////////////////// Context::add_resources ////////////////////////////
  void Context::add_resources(PlatformInterface &platform, AssetManager &assets, Asset const *catalog, leap::string_view fontpath, leap::string_view imagepath)
  {    
    asset_guard lock(assets);

    auto starttime = std::chrono::steady_clock::now();

    while (!assets.request(platform, catalog))
    {
      if ((std::chrono::steady_clock::now() - starttime) > std::chrono::seconds(5))
        throw runtime_error("Timeout Reading Ui Resource Catalog");
    }

    auto bits = assets.request(platform, catalog);

    auto payload = reinterpret_cast<PackCatalogPayload const *>(bits);

    auto entrytable = PackCatalogPayload::entrytable(bits, payload->entrycount);
    auto stringstable = PackCatalogPayload::stringstable(bits, payload->entrycount);

    for(size_t i = 0; i < payload->entrycount; ++i)
    {
      auto path = leap::string_view(stringstable + entrytable[i].pathindex, entrytable[i].pathlength);
      auto name = path.substr(path.find_last_of('/') + 1);
      auto folder = path.substr(0, path.find_last_of('/'));
      auto asset = assets.find(catalog->id + entrytable[i].id);

      if (folder == fontpath)
      {
        fontcatalog.add(name, asset);
      }

      if (folder == imagepath)
      {
        spritecatalog.add(name, asset);
      }
    }
  }


  ///////////////////////// Context::insert_node //////////////////////////////
  Context::Node *Context::insert_node(Node *node, Node *parent)
  {
    assert(parent);
    assert(!node->parent && !node->prevsibling && !node->nextsibling);

    node->parent = parent;

    if (parent->lastchild)
    {
      node->prevsibling = parent->lastchild;
      node->prevsibling->nextsibling = node;
      parent->lastchild = node;
    }
    else
    {
      parent->firstchild = parent->lastchild = node;
    }

    return node;
  }


  ///////////////////////// Context::remove_node //////////////////////////////
  Context::Node *Context::remove_node(Node *node)
  {
    assert(node->parent);

    if (node->parent->firstchild == node)
      node->parent->firstchild = node->nextsibling;

    if (node->parent->lastchild == node)
      node->parent->lastchild = node->prevsibling;

    if (node->prevsibling)
      node->prevsibling->nextsibling = node->nextsibling;

    if (node->nextsibling)
      node->nextsibling->prevsibling = node->prevsibling;

    node->parent = nullptr;
    node->prevsibling = nullptr;
    node->nextsibling = nullptr;

    return node;
  }


  ///////////////////////// Context::destroy //////////////////////////////////
  void Context::destroy(Ui::Item *item)
  {
    auto node = node_cast<Node>(item);

    if (node->parent)
      remove_node(node);

    if (item == pressitem)
      pressitem = nullptr;

    if (item == focusitem)
      focusitem = nullptr;

    if (item == popupitem)
      popupowner = popupitem = nullptr;

    if (item == popupowner)
      destroy(popupitem);

    while (auto child = node->lastchild)
      destroy(item_cast<Ui::Item>(child));

    node->~Node();

    deallocate(m_allocator, reinterpret_cast<char*>(node), node->size);
  }


  ///////////////////////// Context::add_string /////////////////////////////
  const char *Context::add_string(Ui::Item * item, leap::string_view str)
  {
    auto node = node_cast<Node>(item);

    node->strings.emplace_back(str.data(), str.size());

    return node->strings.back().c_str();
  }


  ///////////////////////// Context::update ///////////////////////////////////
  void Context::update(Node *node, GameInput const &input, float dt)
  {
    for(auto child = node->lastchild; child; child = child->prevsibling)
    {
      update(child, input, dt);
    }

    for(auto &animation : node->animations)
    {
      if (animation.running)
        animation.alpha += dt / animation.duration;
    }

    (this->*(node->update))(node, input, dt);
  }


  ///////////////////////// Context::prepare //////////////////////////////////
  void Context::prepare(Node *node)
  {
    auto item = item_cast<Ui::Item>(node);

    for(auto &animation : node->animations)
    {
      if (animation.running && (animation.flags & (Animation::Clip | Animation::Visible)))
      {
        item->clip = animation.oldclip;
        item->visible = animation.oldvisible;
      }
    }

    for(auto &binding : node->bindings)
    {
      switch (binding.type)
      {
        case Ui::Binding::Type::Int:
          *binding.dst_int = binding.value_int;
          break;

        case Ui::Binding::Type::Bool:
          *binding.dst_bool = binding.value_bool;
          break;

        case Ui::Binding::Type::Float:
          *binding.dst_float = binding.value_float;
          break;

        case Ui::Binding::Type::IntReference:
          *binding.dst_int = *binding.reference_int;
          break;

        case Ui::Binding::Type::BoolReference:
          *binding.dst_bool = *binding.reference_bool;
          break;

        case Ui::Binding::Type::FloatReference:
          *binding.dst_float = *binding.reference_float;
          break;

        case Ui::Binding::Type::IntExpression:
          *binding.dst_int = int(evaluate(binding.expression));
          break;

        case Ui::Binding::Type::BoolExpression:
          *binding.dst_bool = !fcmp(evaluate(binding.expression), 0.0f);
          break;

        case Ui::Binding::Type::FloatExpression:
          *binding.dst_float = evaluate(binding.expression);
          break;
      }
    }

    for(auto &animation : node->animations)
    {
      if (animation.flags & Animation::Bound)
      {
        if (*animation.target != animation.finishvalue)
        {
          animation.alpha = animation.running ? 0.016f / animation.duration : 0.0f;
          animation.startvalue = animation.currentvalue;
          animation.finishvalue = *animation.target;
          animation.running = (animation.startvalue != animation.finishvalue) && !fcmp(evaluate(animation.enabled), 0.0f);
        }
      }

      if (animation.flags & Animation::Repeat)
      {
        if (!animation.running)
        {
          animation.alpha = 0.0f;
          animation.running = !fcmp(evaluate(animation.enabled), 0.0f);
        }
      }

      if (animation.running)
      {
        if (animation.alpha > 1.0f - 1e-6f)
        {
          animation.alpha = fmod(animation.alpha, 1.0f);
          animation.running = (animation.flags & Animation::Repeat) && !fcmp(evaluate(animation.enabled), 0.0f);
        }

        if (animation.running)
        {
          animation.oldclip = item->clip;
          animation.oldvisible = item->visible;

          item->clip |= ((animation.flags & Animation::Clip) != 0);
          item->visible |= ((animation.flags & Animation::Visible) != 0);

          *animation.target = lerp(animation.startvalue, animation.finishvalue, animation.alpha);
        }
      }
    }

    for(auto &animation : node->animations)
    {
      animation.currentvalue = *animation.target;
    }

    if (item->visible)
    {
      for(auto child = node->firstchild; child; child = child->nextsibling)
      {
        prepare(child);
      }

      // sort for z order
      for(auto child = node->firstchild; child; child = child->nextsibling)
      {
        while (child->nextsibling && item_cast<Ui::Item>(child->nextsibling)->z < item_cast<Ui::Item>(child)->z)
        {
          auto ip = remove_node(child->nextsibling);

          auto jp = child;
          while (jp->prevsibling && item_cast<Ui::Item>(ip)->z < item_cast<Ui::Item>(jp->prevsibling)->z)
            jp = jp->prevsibling;

          ip->parent = node;
          ip->nextsibling = jp;
          ip->prevsibling = jp->prevsibling;
          if (ip->prevsibling) ip->prevsibling->nextsibling = ip;
          if (ip->nextsibling) ip->nextsibling->prevsibling = ip;
          if (node->firstchild == jp) node->firstchild = ip;
        }
      }
    }
  }


  ///////////////////////// Context::render ///////////////////////////////////
  void Context::render(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node)
  {
    (this->*(node->prepare))(node);

    auto item = item_cast<Ui::Item>(node);

    bool visible = (item->visible && item->opacity != 0);

    if (visible)
    {
      auto cliprect = buildstate.cliprect();

      if (item->clip)
      {
        if (auto newclip = intersection(cliprect, item->rect()))
        {
          spritelist.push_scissor(buildstate, *newclip);
        }
      }

      if (!intersects(item->rect(), buildstate.cliprect()))
      {
        visible = false;
      }

      if (visible)
      {
        (this->*(node->render))(spritelist, buildstate, node);

        if (item->clip)
        {
          spritelist.push_scissor(buildstate, cliprect);
        }

//        spritelist.push_rect_outline(buildstate, Vec2(item->x, item->y), Rect2(Vec2(0), Vec2(item->width, item->height)), item->rotation, Color3(0, 1, 1));
      }
    }
  }


  ///////////////////////// request ///////////////////////////////////////////
  void request(PlatformInterface &platform, Ui::Context &context, Ui::Item *item, int *ready, int *total)
  {
    assert(item);

    auto node = Context::node_cast<Context::Node>(item);

    (context.*(node->request))(platform, node, ready, total);

    for(auto child = node->firstchild; child; child = child->nextsibling)
    {
      request(platform, context, Context::item_cast<Item>(child), ready, total);
    }
  }


  ///////////////////////// update ////////////////////////////////////////////
  void update(Ui::Context &context, Ui::ItemList const &items, GameInput const &input, float dt, bool *accepted)
  {
    assert(accepted);
    assert(!context.pressitem || *accepted); // set accepted to pressitem for correct operation

    context.actions.clear();
    context.mousepos = Vec2(input.mousex, input.mousey);
    context.cursor = Ui::Context::Cursor::Arrow;

    context.inputaccepted = *accepted;

    if (context.popupitem)
    {
      context.ui = context.popupitem;
      context.update(Context::node_cast<Context::Node>(context.popupitem), input, dt);

      *accepted = context.inputaccepted;
    }

    for(auto i = items.rbegin(); i != items.rend(); ++i)
    {
      context.ui = *i;
      context.update(Context::node_cast<Context::Node>(*i), input, dt);

      if (context.inputaccepted && !*accepted)
      {
        context.activeitem = *i;

        *accepted = true;
      }
    }

    if (context.focusitem)
    {
      *accepted = true;
    }
  }


  ///////////////////////// render ////////////////////////////////////////////
  void render(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Context &context, Ui::ItemList const &items)
  {
    context.hoveritem = nullptr;

    for(auto i = items.begin(); i != items.end(); ++i)
    {
      context.prepare(Context::node_cast<Context::Node>(*i));
    }

    if (context.popupitem)
    {
      context.prepare(Context::node_cast<Context::Node>(context.popupitem));
    }

    for(auto i = items.begin(); i != items.end(); ++i)
    {
      context.render(spritelist, buildstate, Context::node_cast<Context::Node>(*i));
    }

    if (context.popupitem)
    {
      context.render(spritelist, buildstate, Context::node_cast<Context::Node>(context.popupitem));
    }
  }
}


///////////////////////// open //////////////////////////////////////////////
void open(Ui::Context &ui, Ui::Item *item, Ui::ItemList &items)
{
  raise(ui, item, items);
}


///////////////////////// show //////////////////////////////////////////////
void show(Ui::Context &ui, Ui::Item *item, bool show)
{
  item->visible = show;

  if (!item->visible && item == ui.activeitem)
  {
    ui.pressitem = nullptr;
    ui.focusitem = nullptr;
    ui.activeitem = nullptr;
  }
}


///////////////////////// raise /////////////////////////////////////////////
void raise(Ui::Context &ui, Ui::Item *item, Ui::ItemList &items)
{
  auto jp = find(items.begin(), items.end(), item);
  if (jp != items.end())
    items.erase(jp);

  auto ip = items.begin();
  while (ip != items.end() && (*ip)->z <= item->z)
    ++ip;

  items.insert(ip, item);
}


///////////////////////// close /////////////////////////////////////////////
void close(Ui::Context &ui, Ui::Item *item, Ui::ItemList &items)
{
  if (item == ui.activeitem)
  {
    ui.pressitem = nullptr;
    ui.focusitem = nullptr;
    ui.activeitem = nullptr;
  }

  auto jp = find(items.begin(), items.end(), item);
  if (jp != items.end())
    items.erase(jp);
}


///////////////////////// update_ui_overlay /////////////////////////////////
void update_ui_overlay(Ui::Context &ui, Ui::ItemList const &items, GameInput const &input, float dt, bool *accepted)
{
  BEGIN_TIMED_BLOCK(UiUpdate, Color3(0.0f, 0.6f, 0.8f))

  update(ui, items, input, dt, accepted);

  END_TIMED_BLOCK(UiUpdate)
}


///////////////////////// render_ui_overlay /////////////////////////////////
void render_ui_overlay(RenderContext &context, ResourceManager &resources, PushBuffer &pushbuffer, Viewport const &viewport, Ui::Context &ui, Ui::ItemList const &items)
{
  BEGIN_TIMED_BLOCK(UiOverlay, Color3(0.5f, 0.7f, 0.8f))

  SpriteList spritelist;
  SpriteList::BuildState buildstate;

  if (spritelist.begin(buildstate, context, resources))
  {
    render(spritelist, buildstate, ui, items);

    spritelist.finalise(buildstate);

    if (auto entry = pushbuffer.push<Renderable::Sprites>())
    {
      entry->spritecommands = spritelist.spritecommands;
    }
  }

  END_TIMED_BLOCK(UiOverlay)
}
