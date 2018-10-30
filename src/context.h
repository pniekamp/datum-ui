//
// datum-ui - context
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/memory.h"
#include "fontcatalog.h"
#include "spritecatalog.h"
#include "expression.h"
#include <functional>
#include <deque>
#include <vector>
#include <unordered_map>

namespace Ui
{
  //|---------------------- Item ----------------------------------------------
  //|--------------------------------------------------------------------------

  struct Item
  {
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;

    float z = 0.0f;

    float scale = 1.0f;
    float rotation = 0.0f;

    float opacity = 1.0f;

    bool clip = false;
    bool visible = true;

    lml::Vec2 pos() const { return { x, y }; }
    lml::Vec2 dim() const { return { width, height }; }
    lml::Rect2 rect() const { return { pos(), pos() + dim() }; }

    lml::Vec2 map(lml::Vec2 const &childpos) const { return pos() + leap::lml::rotate(childpos * scale, rotation); }
    lml::Vec2 unmap(lml::Vec2 const &worldpos) const { return leap::lml::rotate(worldpos - pos(), -rotation); }

    bool contains(lml::Vec2 const &worldpos) const { return leap::lml::contains(lml::Rect2(lml::Vec2(0.0f), dim()), unmap(worldpos)); }
  };

  using ItemList = std::vector<Item*, StackAllocator<Item*>>;


  //|---------------------- Binding -------------------------------------------
  //|--------------------------------------------------------------------------

  struct Binding
  {
    enum class Type
    {
      Int,
      Bool,
      Float,
      IntReference,
      BoolReference,
      FloatReference,
      IntExpression,
      BoolExpression,
      FloatExpression,
    };

    Type type;

    union
    {
      int *dst_int;
      bool *dst_bool;
      float *dst_float;
    };

    union
    {
      int value_int;
      bool value_bool;
      float value_float;
      int const *reference_int;
      bool const *reference_bool;
      float const *reference_float;
      Expression const *expression;
    };

    Binding(int *variable, int value)
      : type(Type::Int), dst_int(variable), value_int(value)
    {
    }

    Binding(bool *variable, bool value)
      : type(Type::Bool), dst_bool(variable), value_bool(value)
    {
    }

    Binding(float *variable, float value)
      : type(Type::Float), dst_float(variable), value_float(value)
    {
    }

    Binding(int *variable, int const *value)
      : type(Type::IntReference), dst_int(variable), reference_int(value)
    {
    }

    Binding(bool *variable, bool const *value)
      : type(Type::BoolReference), dst_bool(variable), reference_bool(value)
    {
    }

    Binding(float *variable, float const *value)
      : type(Type::FloatReference), dst_float(variable), reference_float(value)
    {
    }

    Binding(int *variable, Expression const *expression)
      : type(Type::IntExpression), dst_int(variable), expression(expression)
    {
    }

    Binding(bool *variable, Expression const *expression)
      : type(Type::BoolExpression), dst_bool(variable), expression(expression)
    {
    }

    Binding(float *variable, Expression const *expression)
      : type(Type::FloatExpression), dst_float(variable), expression(expression)
    {
    }
  };


  //|---------------------- Animation -----------------------------------------
  //|--------------------------------------------------------------------------

  struct Animation
  {
    float *target;

    Expression const *enabled;

    enum AnimationFlags
    {
      Bound = 0x01,
      Repeat = 0x02,

      Clip = 0x10,
      Visible = 0x20
    };

    long flags = 0;

    float duration = 0.250f;
    float startvalue = 0.0f;
    float finishvalue = 0.0f;

    float alpha;
    bool running;
    bool oldclip;
    bool oldvisible;
    float currentvalue;

    Animation(float *target, Expression const *enabled, long flags, float duration)
      : target(target), enabled(enabled), flags(flags), duration(duration)
    {
      running = false;
      finishvalue = *target;
    }
  };


  //|---------------------- Context -------------------------------------------
  //|--------------------------------------------------------------------------

  class Context
  {
    public:

      using allocator_type = StackAllocator<>;

      Context(ResourceManager &resources, allocator_type const &allocator);

      Context(Context const &) = delete;

    public:

      FontCatalog fontcatalog;
      SpriteCatalog spritecatalog;

      void add_resources(DatumPlatform::PlatformInterface &platform, AssetManager &assets, Asset const *catalog, leap::string_view fontpath = "fonts", leap::string_view imagepath = "images");

    public:

      template<typename Item>
      Item *create_item(Ui::Item *parent = nullptr);

      template<typename T>
      void bind(Ui::Item *item, T &variable, std::decay_t<T> const &value);

      template<typename T>
      void bind(Ui::Item *item, T &variable, T *value);

      template<typename T>
      void bind(Ui::Item *item, T &variable, Expression const &value);

      template<typename T>
      Animation *animate(Ui::Item *item, T &variable, Expression const &enabled = StackExpression<5>{true}, long flags = 0, float duration = 0.250f);

      void destroy(Ui::Item *item);

    public:

      struct Action
      {
        int id;
        int op;
        Ui::Item *ui;
        Ui::Item *item;
      };

      std::vector<Action, StackAllocator<Action>> actions;

      Ui::Item *hoveritem;
      Ui::Item *pressitem;
      Ui::Item *focusitem;
      Ui::Item *activeitem;

      Ui::Item *popupitem;
      Ui::Item *popupowner;

      enum class Cursor
      {
        Arrow,
        Edit,
        Hide,
      };

      Cursor cursor;

    public:

      const char *add_string(Ui::Item *item, leap::string_view str);

    private:

      FreeList m_freelist;
      StackAllocatorWithFreelist<> m_allocator;

    private:

      bool inputaccepted;

      lml::Vec2 mousepos;
      lml::Vec2 presspos;
      bool dragging;
      float elapsed;
      Ui::Item *timeditem;

      Ui::Item *ui;

    private:

      using String = std::basic_string<char, std::char_traits<char>, StackAllocatorWithFreelist<char>>;
      using Strings = std::deque<String, std::scoped_allocator_adaptor<StackAllocatorWithFreelist<String>>>;
      using Bindings = std::vector<Binding, StackAllocatorWithFreelist<Binding>>;
      using Animations = std::deque<Animation, StackAllocatorWithFreelist<Animation>>;

      struct Node
      {
        size_t size;

        Node *parent = nullptr;
        Node *firstchild = nullptr;
        Node *lastchild = nullptr;
        Node *nextsibling = nullptr;
        Node *prevsibling = nullptr;

        Strings strings;
        Bindings bindings;
        Animations animations;

        void (Context::*update)(Node*, DatumPlatform::GameInput const &, float);
        void (Context::*request)(DatumPlatform::PlatformInterface &, Node *node, int*, int*);
        void (Context::*prepare)(Node*);
        void (Context::*render)(SpriteList&, SpriteList::BuildState&, Node*);

        std::deque<BasicExpression<StackAllocatorWithFreelist<uint8_t>>, std::scoped_allocator_adaptor<StackAllocatorWithFreelist<BasicExpression<StackAllocatorWithFreelist<uint8_t>>>>> expressions;

        Node(size_t size, StackAllocatorWithFreelist<> const &allocator)
          : size(size),
            strings(allocator),
            bindings(allocator),
            animations(allocator),
            expressions(allocator)
        {
        }
      };

      template<typename Item>
      static Item *item_cast(Node *node)
      {
        return reinterpret_cast<Item*>((char*)node + leap::alignto(sizeof(Node), alignof(std::max_align_t)));
      }

      template<typename Node>
      static Node *node_cast(Item *item)
      {
        return reinterpret_cast<Node*>((char*)item - leap::alignto(sizeof(Node), alignof(std::max_align_t)));
      }

      Node *insert_node(Node *node, Node *parent);
      Node *remove_node(Node *node);

      template<typename Item>
      void update_item(Node *node, DatumPlatform::GameInput const &input, float dt);

      template<typename Item>
      void request_item(DatumPlatform::PlatformInterface &platform, Node *node, int *ready, int *total);

      template<typename Item>
      void prepare_item(Node *node);

      template<typename Item>
      void render_item(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node);

      void update(Node *node, DatumPlatform::GameInput const &input, float dt);
      void prepare(Node *node);
      void render(SpriteList &spritelist, SpriteList::BuildState &buildstate, Node *node);

      friend void request(DatumPlatform::PlatformInterface &platform, Ui::Context &ui, Ui::Item *item, int *ready, int *total);

      friend void update(Ui::Context &ui, Ui::ItemList const &items, DatumPlatform::GameInput const &input, float dt, bool *accepted);
      friend void render(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Context &ui, Ui::ItemList const &items);
  };

  template<typename Item>
  Item *Context::create_item(Ui::Item *parent)
  {
    size_t size = leap::alignto(sizeof(Node), alignof(std::max_align_t)) + sizeof(Item);

    auto node = new(allocate<char>(m_allocator, size, alignof(std::max_align_t))) Node(size, m_allocator);

    if (parent)
    {
      insert_node(node, node_cast<Node>(parent));
    }

    node->update = &Ui::Context::update_item<Item>;
    node->request = &Ui::Context::request_item<Item>;
    node->prepare = &Ui::Context::prepare_item<Item>;
    node->render = &Ui::Context::render_item<Item>;

    return new(item_cast<Item>(node)) Item;
  }

  template<typename T>
  void Context::bind(Ui::Item *item, T &variable, std::decay_t<T> const &value)
  {
    auto node = node_cast<Node>(item);

    node->bindings.emplace_back(&variable, value);

    variable = value;
  }

  template<typename T>
  void Context::bind(Ui::Item *item, T &variable, T *value)
  {
    auto node = node_cast<Node>(item);

    node->bindings.emplace_back(&variable, value);

    variable = *value;
  }

  template<typename T>
  void Context::bind(Ui::Item *item, T &variable, Expression const &value)
  {
    auto node = node_cast<Node>(item);

    node->expressions.emplace_back(value);

    node->bindings.emplace_back(&variable, &node->expressions.back());

    variable = (T)evaluate(value);
  }

  template<typename T>
  Animation *Context::animate(Ui::Item *item, T &variable, Expression const &enabled, long flags, float duration)
  {
    auto node = node_cast<Node>(item);

    node->expressions.emplace_back(enabled);

    node->animations.emplace_back(&variable, &node->expressions.back(), flags, duration);

    return &node->animations.back();
  }

  // Functions

  void request(DatumPlatform::PlatformInterface &platform, Ui::Context &ui, Ui::Item *item, int *ready, int *total);

  void update(Ui::Context &ui, Ui::ItemList const &items, DatumPlatform::GameInput const &input, float dt, bool *accepted);
  void render(SpriteList &spritelist, SpriteList::BuildState &buildstate, Ui::Context &ui, Ui::ItemList const &items);
}

using UiItem = Ui::Item;
using UiItemList = Ui::ItemList;
using UiContext = Ui::Context;

void open(Ui::Context &ui, Ui::Item *item, Ui::ItemList &items);
void show(Ui::Context &ui, Ui::Item *item, bool show);
void raise(Ui::Context &ui, Ui::Item *item, Ui::ItemList &items);
void close(Ui::Context &ui, Ui::Item *item, Ui::ItemList &items);

void update_ui_overlay(Ui::Context &ui, Ui::ItemList const &items, DatumPlatform::GameInput const &input, float dt, bool *accepted);
void render_ui_overlay(RenderContext &context, ResourceManager &resources, PushBuffer &pushbuffer, DatumPlatform::Viewport const &viewport, Ui::Context &ui, Ui::ItemList const &items);
