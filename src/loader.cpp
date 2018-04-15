//
// datum-ui - loader
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "loader.h"
#include "compiler_p.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;

namespace
{
  float parse_decimal(leap::string_view str)
  {
    float result = 0;

    auto ch = str.begin();

    while (ch != str.end() && *ch >= '0' && *ch <= '9')
    {
      result = result * 10 + (*ch - '0');

      ++ch;
    }

    if (ch != str.end() && *ch == '.')
    {
      ++ch;

      float divisor = 10;
      while (ch != str.end() && *ch >= '0' && *ch <= '9')
      {
        result = result + (*ch - '0') / divisor;
        divisor = divisor * 10;

        ++ch;
      }
    }

    return result;
  }
}

namespace
{
  struct Loader
  {
    Loader(Ui::Context &context, StackAllocator<> const &allocator)
      : context(context),
        itemstack(allocator),
        currentexpr(allocator),
        itemlist(allocator)
    {
    }

    ///////////////////////// create_item /////////////////////////////////////
    size_t create_item(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      currentitem = Ui::create_item(context, id, currentitem);

      itemstack.push_back(currentitem);

      return sizeof(Ui::Op) + sizeof(Ui::Id);
    }

    ///////////////////////// find_item ///////////////////////////////////////
    Ui::Item *find_item(int index)
    {
      Ui::Item *item = nullptr;

      if (index <= 0)
      {
        if (size_t(1 - index) > itemstack.size())
          throw runtime_error("Invalid Stack Index");

        item = itemstack[itemstack.size() + index - 1];
      }

      if (index > 0)
      {
        if (size_t(index) > itemlist.size())
          throw runtime_error("Invalid List Index");

        item = itemlist[index - 1].second;
      }

      return item;
    }

    ///////////////////////// set_name ////////////////////////////////////////
    size_t set_id(uint8_t const *ptr)
    {
      uint32_t size;
      memcpy(&size, ptr + 1, sizeof(uint32_t));

      auto sv = leap::string_view((char const *)ptr + 5, size);

      itemlist.emplace_back(sv, currentitem);

      return sizeof(Ui::Op) + sizeof(uint32_t) + size;
    }

    ///////////////////////// store_int ///////////////////////////////////////
    size_t store_int(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      int32_t value;
      memcpy(&value, ptr + 3, sizeof(int32_t));

      if (Ui::always_bind(id))
      {
        context.bind(currentitem, *Ui::address_of_int(currentitem, id), value);
      }
      else
      {
        *Ui::address_of_int(currentitem, id) = value;
      }

      return sizeof(Ui::Op) + sizeof(Ui::Id) + sizeof(int32_t);
    }

    ///////////////////////// store_bool //////////////////////////////////////
    size_t store_bool(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      int32_t value;
      memcpy(&value, ptr + 3, sizeof(int32_t));

      if (Ui::always_bind(id))
      {
        context.bind(currentitem, *Ui::address_of_bool(currentitem, id), value);
      }
      else
      {
        *Ui::address_of_bool(currentitem, id) = value;
      }

      return sizeof(Ui::Op) + sizeof(Ui::Id) + sizeof(int32_t);
    }

    ///////////////////////// store_float /////////////////////////////////////
    size_t store_float(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      float value;
      memcpy(&value, ptr + 3, sizeof(float));

      if (Ui::always_bind(id))
      {
        context.bind(currentitem, *Ui::address_of_float(currentitem, id), value);
      }
      else
      {
        *Ui::address_of_float(currentitem, id) = value;
      }

      return sizeof(Ui::Op) + sizeof(Ui::Id) + sizeof(float);
    }

    ///////////////////////// store_color /////////////////////////////////////
    size_t store_color(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      Color4 value;
      memcpy(&value, ptr + 3, sizeof(Color4));

      *Ui::address_of_color(currentitem, id) = value;

      return sizeof(Ui::Op) + sizeof(Ui::Id) + sizeof(Color4);
    }

    ///////////////////////// store_text //////////////////////////////////////
    size_t store_text(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      uint32_t size;
      memcpy(&size, ptr + 3, sizeof(uint32_t));

      auto sv = leap::string_view((char const *)ptr + 7, size);

      switch (Ui::type_of(id))
      {
        case Ui::Type::Font:
        {
          auto j = sv.find_first_of(':');

          auto face = trim(sv.substr(0, j));
          auto size = 16.0f;

          if (j != leap::string_view::npos)
          {
            size = parse_decimal(trim(sv.substr(j + 1)));
          }

          *Ui::address_of_font(currentitem, id) = context.fontcatalog.find(face, size);
          break;
        }

        case Ui::Type::Sprite:
        {
          auto j = sv.find_first_of(':');

          auto name = trim(sv.substr(0, j));
          auto region = Rect2(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f));

          if (j != leap::string_view::npos)
          {
            size_t fields[5];
            fields[0] = j + 1;
            fields[1] = min(sv.find_first_of(',', fields[0]), sv.size() - 1) + 1;
            fields[2] = min(sv.find_first_of(',', fields[1]), sv.size() - 1) + 1;
            fields[3] = min(sv.find_first_of(',', fields[2]), sv.size() - 1) + 1;
            fields[4] = sv.size() + 1;

            region.min.x = parse_decimal(trim(sv.substr(fields[0], fields[1] - fields[0] - 1)));
            region.min.y = parse_decimal(trim(sv.substr(fields[1], fields[2] - fields[1] - 1)));
            region.max.x = parse_decimal(trim(sv.substr(fields[2], fields[3] - fields[2] - 1)));
            region.max.y = parse_decimal(trim(sv.substr(fields[3], fields[4] - fields[3] - 1)));
          }

          *Ui::address_of_sprite(currentitem, id) = context.spritecatalog.find(name, region);
          break;
        }

        case Ui::Type::Text:
          *Ui::address_of_text(currentitem, id) = context.add_string(currentitem, sv);
          break;

        default:
          throw runtime_error("Invalid Text Type");
      }

      return sizeof(Ui::Op) + sizeof(Ui::Id) + sizeof(uint32_t) + size;
    }

    ///////////////////////// begin_expr //////////////////////////////////////
    size_t begin_expr(uint8_t const *ptr)
    {
      uint32_t size;
      memcpy(&size, ptr + 1, sizeof(uint32_t));

      currentexpr.clear();
      currentexpr.push(ptr + 5, size);

      return sizeof(Ui::Op) + sizeof(uint32_t) + size;
    }

    ///////////////////////// patch_expr //////////////////////////////////////
    size_t patch_expr(uint8_t const *ptr)
    {
      uint32_t offset;
      memcpy(&offset, ptr + 1, sizeof(uint32_t));

      int16_t index;
      memcpy(&index, ptr + 5, sizeof(int16_t));

      Ui::Id id;
      memcpy(&id, ptr + 7, sizeof(id));

      switch (Ui::type_of(id))
      {
        case Ui::Type::Int:
          currentexpr.patch(offset, Ui::address_of_int(find_item(index), id));
          break;

        case Ui::Type::Bool:
          currentexpr.patch(offset, Ui::address_of_bool(find_item(index), id));
          break;

        case Ui::Type::Float:
          currentexpr.patch(offset, Ui::address_of_float(find_item(index), id));
          break;

        default:
          throw runtime_error("Invalid Property Type");
      }

      return sizeof(Ui::Op) + sizeof(uint32_t) + sizeof(int16_t) + sizeof(Ui::Id);
    }

    ///////////////////////// store_expr //////////////////////////////////////
    size_t store_expr(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      switch (Ui::type_of(id))
      {
        case Ui::Type::Int:
          context.bind(currentitem, *Ui::address_of_int(currentitem, id), currentexpr);
          break;

        case Ui::Type::Bool:
          context.bind(currentitem, *Ui::address_of_bool(currentitem, id), currentexpr);
          break;

        case Ui::Type::Float:
          context.bind(currentitem, *Ui::address_of_float(currentitem, id), currentexpr);
          break;

        default:
          throw runtime_error("Invalid Expression Type");
      }

      return sizeof(Ui::Op) + sizeof(Ui::Id);
    }

    ///////////////////////// store_anim //////////////////////////////////////
    size_t store_anim(uint8_t const *ptr)
    {
      int16_t index;
      memcpy(&index, ptr + 1, sizeof(int16_t));

      Ui::Id id;
      memcpy(&id, ptr + 3, sizeof(id));

      int32_t flags;
      memcpy(&flags, ptr + 5, sizeof(int32_t));

      float duration;
      memcpy(&duration, ptr + 9, sizeof(float));

      Ui::Animation *anim;

      switch (Ui::type_of(id))
      {
        case Ui::Type::Float:
          anim = context.animate(currentitem, *Ui::address_of_float(find_item(index), id), currentexpr, flags, duration);
          break;

        default:
          throw runtime_error("Invalid Animation Type");
      }

      if (!(flags & Ui::Animation::Bound))
      {
        memcpy(&anim->startvalue, ptr + 13, sizeof(float));
        memcpy(&anim->finishvalue, ptr + 17, sizeof(float));
      }

      return sizeof(Ui::Op) + sizeof(int16_t) + sizeof(Ui::Id) + sizeof(int32_t) + 3*sizeof(float);
    }

    ///////////////////////// pop /////////////////////////////////////////////
    size_t pop(uint8_t const *&ptr)
    {
      itemstack.pop_back();

      if (itemstack.size() != 0)
        currentitem = itemstack.back();

      return sizeof(Ui::Op);
    }

    ///////////////////////// load ////////////////////////////////////////////
    Ui::Item *load(uint8_t const *bytecode, size_t length, LoaderItemList *items)
    {
      for(uint8_t const *ptr = bytecode; ptr < bytecode + length; )
      {
        switch (static_cast<Ui::Op>(*ptr))
        {
          case Ui::Op::CreateItem:
            ptr += create_item(ptr);
            break;

          case Ui::Op::SetId:
            ptr += set_id(ptr);
            break;

          case Ui::Op::StoreInt:
            ptr += store_int(ptr);
            break;

          case Ui::Op::StoreBool:
            ptr += store_bool(ptr);
            break;

          case Ui::Op::StoreFloat:
            ptr += store_float(ptr);
            break;

          case Ui::Op::StoreColor:
            ptr += store_color(ptr);
            break;

          case Ui::Op::StoreText:
            ptr += store_text(ptr);
            break;

          case Ui::Op::BeginExpr:
            ptr += begin_expr(ptr);
            break;

          case Ui::Op::PatchExpr:
            ptr += patch_expr(ptr);
            break;

          case Ui::Op::StoreExpr:
            ptr += store_expr(ptr);
            break;

          case Ui::Op::StoreAnim:
            ptr += store_anim(ptr);
            break;

          case Ui::Op::Pop:
            ptr += pop(ptr);
            break;
        }
      }

      if (items)
      {
        for(auto &item : itemlist)
        {
          items->emplace(std::piecewise_construct, forward_as_tuple(item.first.data(), item.first.size()), forward_as_tuple(item.second));
        }
      }

      if (itemstack.size() != 1)
        throw runtime_error("Invalid Ui");

      return itemstack.back();
    }

    private:

      Ui::Context &context;

      Ui::Item *currentitem = nullptr;
      vector<Ui::Item*, StackAllocator<Ui::Item*>> itemstack;
      Ui::basic_expression<StackAllocator<uint8_t>> currentexpr;

      vector<pair<leap::string_view, Ui::Item*>, StackAllocator<pair<leap::string_view, Ui::Item*>>> itemlist;
  };
}

///////////////////////// create_ui /////////////////////////////////////////
Ui::Item *create_ui(Ui::Context &context, uint8_t const *bytecode, size_t length, LoaderItemList *items, Arena &scratchmemory)
{
  return Loader(context, scratchmemory).load(bytecode, length, items);
}

Ui::Item *create_ui(Ui::Context &context, uint8_t const *bytecode, size_t length, Arena &scratchmemory)
{
  return Loader(context, scratchmemory).load(bytecode, length, nullptr);
}
