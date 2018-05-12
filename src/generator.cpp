//
// datum-ui - codegen
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "generator.h"
#include "compiler_p.h"
#include <fstream>
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace leap;

namespace
{
  using leap::toa;

  string toa(float value)
  {
    string str = leap::toa(value);

    if (str.find('.') == string::npos)
      str += '.';

    str += 'f';

    return str;
  }

  string toa(bool value)
  {
    return (value) ? "true" : "false";
  }

}

namespace
{
  struct CodeGen
  {
    ///////////////////////// create_item /////////////////////////////////////
    size_t create_item(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      currenttype = Ui::name_of(id);
      currentitem = tolower(currenttype) + toa(counter++);

      if (itemstack.empty())
        createstr += "  auto " + currentitem + " = context.create_item<Ui::" + currenttype + ">(parent);\n";
      else
        createstr += "\n  auto " + currentitem + " = context.create_item<Ui::" + currenttype + ">(" + itemstack.back() + ");\n";

      itemstack.push_back(currentitem);

      return sizeof(Ui::Op) + sizeof(Ui::Id);
    }

    ///////////////////////// find_item ///////////////////////////////////////
    string find_item(int index)
    {
      string item;

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

    ///////////////////////// set_id //////////////////////////////////////////
    size_t set_id(uint8_t const *ptr)
    {
      uint32_t size;
      memcpy(&size, ptr + 1, sizeof(uint32_t));

      auto name = string((char const *)ptr + 5, size);

      structstr += "    " + currenttype + " *" + name + ";\n";

      itemlist.emplace_back(name, currentitem);

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
        createstr += "  context.bind(" + currentitem + ", " + currentitem + "->" + Ui::name_of(id) + ", " + toa(value) + ");\n";
      }
      else
      {
        createstr += "  " + currentitem + "->" + Ui::name_of(id) + " = " + toa(value) + ";\n";
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
        createstr += "  context.bind(" + currentitem + ", " + currentitem + "->" + Ui::name_of(id) + ", " + toa(value != 0) + ");\n";
      }
      else
      {
        createstr += "  " + currentitem + "->" + Ui::name_of(id) + " = " + toa(value != 0) + ";\n";
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
        createstr += "  context.bind(" + currentitem + ", " + currentitem + "->" + Ui::name_of(id) + ", " + toa(value) + ");\n";
      }
      else
      {
        createstr += "  " + currentitem + "->" + Ui::name_of(id) + " = " + toa(value) + ";\n";
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

      createstr += "  " + currentitem + "->" + Ui::name_of(id) + " = lml::Color4(" + toa(value.r) + ", " + toa(value.g) + ", " + toa(value.b) + ", " + toa(value.a) + ");\n";

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
            size = ato<float>(trim(sv.substr(j + 1)).to_string());
          }

          createstr += "  " + currentitem + "->" + Ui::name_of(id) + " = context.fontcatalog.find(\"" + face.to_string() + "\", " + toa(size) + ");\n";
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

            region.min.x = ato<float>(trim(sv.substr(fields[0], fields[1] - fields[0] - 1)));
            region.min.y = ato<float>(trim(sv.substr(fields[1], fields[2] - fields[1] - 1)));
            region.max.x = ato<float>(trim(sv.substr(fields[2], fields[3] - fields[2] - 1)));
            region.max.y = ato<float>(trim(sv.substr(fields[3], fields[4] - fields[3] - 1)));
          }

          createstr += "  " + currentitem + "->" + Ui::name_of(id) + " = context.spritecatalog.find(\"" + name.to_string() + "\", lml::Rect2(lml::Vec2(" + toa(region.min.x) + ", " + toa(region.min.y) + "), lml::Vec2(" + toa(region.max.x) + ", " + toa(region.max.y) + ")));\n";
          break;
        }

        case Ui::Type::Text:
        {
          string text = "";

          for(auto ch : sv)
          {
            switch(ch)
            {
              case '\n':
                text += "\\n";
                break;

              case '"':
                text += "\\\"";
                break;

              case '\\':
                text += "\\\\";
                break;

              default:
                text += ch;
            }
          }

          createstr += "  " + currentitem + "->" + Ui::name_of(id) + " = context.add_string(" + currentitem + ", \"" + text + "\");\n";
          break;
        }

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

      currentexpr = "expr" + toa(counter++);

      createstr += "  auto " + currentexpr + " = Ui::stack_expression<" + toa(size) + ">{ ";
      for(size_t i = 0; i < size; ++i)
        createstr += ((i != 0) ? ", " : "") + toa((int)*(ptr + 5 + i));
      createstr += " };\n";

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

      createstr += "  " + currentexpr + ".patch(" + toa(offset) + ", &" + find_item(index) + "->" + Ui::name_of(id) + ");\n";

      return sizeof(Ui::Op) + sizeof(uint32_t) + sizeof(int16_t) + sizeof(Ui::Id);
    }

    ///////////////////////// store_expr //////////////////////////////////////
    size_t store_expr(uint8_t const *ptr)
    {
      Ui::Id id;
      memcpy(&id, ptr + 1, sizeof(id));

      createstr += "  context.bind(" + currentitem + ", " + currentitem + "->" + Ui::name_of(id) + ", " + currentexpr + ");\n";

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

      float startvalue;
      memcpy(&startvalue, ptr + 13, sizeof(float));

      float finishvalue;
      memcpy(&finishvalue, ptr + 17, sizeof(float));

      auto anim = "anim" + toa(counter++);

      createstr += "  auto " + anim + " = context.animate(" + currentitem + ", " + find_item(index) + "->" + Ui::name_of(id) + ", " + currentexpr + ", " + toa(flags) + ", " + toa(duration) + ");\n";

      if (!(flags & Ui::Animation::Bound))
      {
        createstr += "  " + anim + "->startvalue = " + toa(startvalue) + ";\n";
        createstr += "  " + anim + "->finishvalue = " + toa(finishvalue) + ";\n";
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

    ///////////////////////// codegen /////////////////////////////////////////
    void codegen(string const &name, uint8_t const *bytecode, size_t length)
    {
      structstr = "";
      structstr += "namespace Ui\n";
      structstr += "{\n";
      structstr += "  struct " + name + "\n";
      structstr += "  {\n";

      createstr = "";
      createstr += "template<typename UI>\n";
      createstr += "Ui::Item *create_ui(Ui::Context &context, Ui::Item *parent = nullptr);\n";
      createstr += "\n";
      createstr += "template<typename UI>\n";
      createstr += "Ui::Item *create_ui(Ui::Context &context, UI *ui, Ui::Item *parent = nullptr);\n";
      createstr += "\n";
      createstr += "template<>\n";
      createstr += "Ui::Item *create_ui<Ui::" + name + ">(Ui::Context &context, Ui::" + name + " *ui, Ui::Item *parent)\n";
      createstr += "{\n";

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

      if (itemstack.size() != 1)
        throw runtime_error("Invalid Ui");

      structstr += "  };\n";
      structstr += "}\n";

      createstr += "  \n";
      createstr += "  if (ui)\n";
      createstr += "  {\n";
      for(auto &item : itemlist)
        createstr += "    ui->" + item.first + " = " + item.second + ";\n";
      createstr += "  }\n";
      createstr += "  \n";
      createstr += "  return " + itemstack.back() + ";\n";

      createstr += "};\n";

      createstr += "\n";
      createstr += "template<>\n";
      createstr += "Ui::Item *create_ui<Ui::" + name + ">(Ui::Context &context, Ui::Item *parent)\n";
      createstr += "{\n";
      createstr += "  Ui::" + name + " ui;\n";
      createstr += "\n";
      createstr += "  return create_ui(context, &ui, parent);\n";
      createstr += "};\n";
    }

    string structstr;
    string createstr;

    private:

      size_t counter = 1;

      string currenttype;
      string currentitem;
      string currentexpr;
      vector<string> itemstack;
      vector<pair<string, string>> itemlist;
  };
}

///////////////////////// codegen_ui ////////////////////////////////////////
void codegen_ui(string const &filename, string const &name, uint8_t const *bytecode, size_t length)
{
  CodeGen generator;

  generator.codegen(name, bytecode, length);

  ofstream fout(filename);

  if (!fout)
    throw runtime_error("Unable to create output file");

  fout << "//" << '\n';
  fout << "// datum-ui - generated ui file" << '\n';
  fout << "//" << '\n';
  fout << '\n';
  fout << "#pragma once" << '\n';
  fout << "#pragma GCC diagnostic push" << '\n';
  fout << "#pragma GCC diagnostic ignored \"-Wunused-variable\"" << '\n';
  fout << "#pragma GCC diagnostic ignored \"-Wunused-function\"" << '\n';
  fout << '\n';
  fout << "#include \"datum/ui.h\"" << '\n';
  fout << '\n';

  fout << generator.structstr << '\n';

  fout << "namespace {\n\n";

  fout << generator.createstr << '\n';

  fout << "}" << '\n';

  fout << '\n';
  fout << "#pragma GCC diagnostic pop" << '\n';
  fout << '\n';
}
