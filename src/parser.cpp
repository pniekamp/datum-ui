//
// datum-ui - parser
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "parser.h"
#include "compiler_p.h"
#include "expression.h"
#include <leap/sapstream.h>
#include <stack>
#include <map>
#include <regex>
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace leap;

namespace
{
  struct Parser
  {
    Parser()
    {
      Ui::init_ids_map(ids);
      Ui::init_constants_map(constants);
    }

    ///////////////////////// id //////////////////////////////////////////////

    Ui::Id parse_id(string const &name)
    {
      auto j = ids.find(name);

      if (j == ids.end())
        throw runtime_error(string("Unknown Id : ") + name);

      return j->second;
    }

    ///////////////////////// literals ////////////////////////////////////////

    bool is_literal(string const &str)
    {
      bool numeric = regex_search(str, regex(R"(^[0-9]*\.?[0-9]+([eE][+-]?[0-9]+)?$)"));
      bool boolean = regex_search(str, regex(R"(^(true|false)$)"));
      bool color = regex_search(str, regex(R"(^#[0-9a-fA-F]+$)"));
      bool text = regex_search(str, regex(R"(^"(.|\n)*"$)"));
      bool constant = (constants.find(str) != constants.end());

      return (numeric || boolean || color || text || constant);
    }

    int parse_int_literal(string const &str)
    {
      if (constants.find(str) != constants.end())
        return constants.find(str)->second;
      else
        return ato<int>(str);
    }

    bool parse_bool_literal(string const &str)
    {
      return (str == "true" || str == "1");
    }

    float parse_float_literal(string const &str)
    {
      return ato<float>(str);
    }

    Color4 parse_color_literal(string const &str)
    {
      auto hx = [](char c) -> int { return isdigit(c) ? (c - '0') : (10 + tolower(c) - 'a'); };

      auto color = Color4(1, 1, 1, 1);

      if (str.size() == 4)
      {
        color.r = hx(str[1]) / 15.0f;
        color.g = hx(str[2]) / 15.0f;
        color.b = hx(str[3]) / 15.0f;
      }

      if (str.size() == 5)
      {
        color.a = hx(str[1]) / 15.0f;
        color.r = hx(str[2]) / 15.0f;
        color.g = hx(str[3]) / 15.0f;
        color.b = hx(str[4]) / 15.0f;
      }

      if (str.size() == 7)
      {
        color.r = (hx(str[1]) * 16 + hx(str[2])) / 255.0f;
        color.g = (hx(str[3]) * 16 + hx(str[4])) / 255.0f;
        color.b = (hx(str[5]) * 16 + hx(str[6])) / 255.0f;
      }

      if (str.size() == 9)
      {
        color.a = (hx(str[1]) * 16 + hx(str[2])) / 255.0f;
        color.r = (hx(str[3]) * 16 + hx(str[4])) / 255.0f;
        color.g = (hx(str[5]) * 16 + hx(str[6])) / 255.0f;
        color.b = (hx(str[7]) * 16 + hx(str[8])) / 255.0f;
      }

      return color;
    }

    string parse_text_literal(string const &str)
    {
      return str.substr(1, str.length()-2);
    }

    ///////////////////////// expression //////////////////////////////////////

    struct ParsedExpression
    {
      Ui::BasicExpression<std::allocator<uint8_t>> bytecode;

      vector<pair<size_t, string>> patchtable;

      void push(Ui::Expression::OpCode op)
      {
        bytecode.push(op);
      }

      void push(float value)
      {
        bytecode.push(value);
      }

      void push(string const &variable)
      {
        patchtable.emplace_back(bytecode.size(), variable);

        bytecode.push((float*)nullptr);
      }
    };

    ParsedExpression parse_expression(string const &expression)
    {
      enum class TokenType
      {
        NoToken,
        OpToken,
        Literal,
        Identifier
      };

      enum class OpType
      {
        NoOp,
        PrefixOp,
        InfixOp
      };

      struct Operator
      {
        Ui::Expression::OpCode code;
        size_t order;
        size_t precedence;
      };

      auto match_operator = [](string const &op, OpType optype) {
        if (op == "%") return Operator{Ui::Expression::OpCode::mod, 2, 1};
        if (op == "/") return Operator{Ui::Expression::OpCode::div, 2, 1};
        if (op == "*") return Operator{Ui::Expression::OpCode::mul, 2, 1};
        if (op == "abs") return Operator{Ui::Expression::OpCode::abs, 1, 1};
        if (op == "min") return Operator{Ui::Expression::OpCode::min, 2, 1};
        if (op == "max") return Operator{Ui::Expression::OpCode::max, 2, 1};
        if (op == "floor") return Operator{Ui::Expression::OpCode::floor, 1, 1};
        if (op == "ceil") return Operator{Ui::Expression::OpCode::ceil, 1, 1};
        if (op == "round") return Operator{Ui::Expression::OpCode::round, 1, 1};
        if (op == "trunc") return Operator{Ui::Expression::OpCode::trunc, 1, 1};
        if (op == "clamp") return Operator{Ui::Expression::OpCode::clamp, 3, 1};
        if (op == "sin") return Operator{Ui::Expression::OpCode::sin, 1, 1};
        if (op == "cos") return Operator{Ui::Expression::OpCode::cos, 1, 1};
        if (op == "tan") return Operator{Ui::Expression::OpCode::tan, 1, 1};
        if (op == "asin") return Operator{Ui::Expression::OpCode::asin, 1, 1};
        if (op == "acos") return Operator{Ui::Expression::OpCode::acos, 1, 1};
        if (op == "atan") return Operator{Ui::Expression::OpCode::atan, 1, 1};
        if (op == "atan2") return Operator{Ui::Expression::OpCode::atan2, 2, 1};
        if (op == "pow") return Operator{Ui::Expression::OpCode::pow, 2, 1};
        if (op == "sqrt") return Operator{Ui::Expression::OpCode::sqrt, 1, 1};
        if (op == "log") return Operator{Ui::Expression::OpCode::log, 1, 1};
        if (op == "exp") return Operator{Ui::Expression::OpCode::exp, 1, 1};
        if (op == "log2") return Operator{Ui::Expression::OpCode::log2, 1, 1};
        if (op == "exp2") return Operator{Ui::Expression::OpCode::exp2, 1, 1};
        if (op == "if") return Operator{Ui::Expression::OpCode::cond, 3, 1};
        if (op == "+" && optype == OpType::PrefixOp) return Operator{Ui::Expression::OpCode::plus, 1, 2};
        if (op == "-" && optype == OpType::PrefixOp) return Operator{Ui::Expression::OpCode::minus, 1, 2};
        if (op == "+") return Operator{Ui::Expression::OpCode::add, 2, 2};
        if (op == "-") return Operator{Ui::Expression::OpCode::sub, 2, 2};
        if (op == "<=") return Operator{Ui::Expression::OpCode::leq, 2, 3};
        if (op == ">=") return Operator{Ui::Expression::OpCode::geq, 2, 3};
        if (op == "<") return Operator{Ui::Expression::OpCode::le, 2, 3};
        if (op == ">") return Operator{Ui::Expression::OpCode::ge, 2, 3};
        if (op == "==") return Operator{Ui::Expression::OpCode::eq, 2, 4};
        if (op == "!=") return Operator{Ui::Expression::OpCode::neq, 2, 4};
        if (op == "!") return Operator{Ui::Expression::OpCode::bnot, 1, 5};
        if (op == "&&") return Operator{Ui::Expression::OpCode::band, 2, 5};
        if (op == "||") return Operator{Ui::Expression::OpCode::bor, 2, 5};
        if (op == "(") return Operator{static_cast<Ui::Expression::OpCode>(252), 0, 6};
        if (op == ")") return Operator{static_cast<Ui::Expression::OpCode>(253), 0, 6};
        if (op == ",") return Operator{static_cast<Ui::Expression::OpCode>(254), 0, 6};
        return Operator{static_cast<Ui::Expression::OpCode>(255), 0, 0};
      };

      regex token_regex(R"(([0-9]*\.?[0-9]+([eE][+-]?[0-9]+)?)|([a-zA-Z]+[a-zA-Z0-9\.:_]*)|([%/\*\+\-]|<=|>=|==|\!=|[\!<>]|\&\&|\|\||[\(\)\,]))");

      ParsedExpression result;

      stack<Operator> operatorstack;

      size_t pos = 0;
      OpType nextop = OpType::PrefixOp;

      while (pos < expression.size())
      {
        while (pos < expression.size() && expression[pos] <= ' ')
          ++pos;

        if (pos == expression.size())
          break;

        smatch token_match;
        if (!regex_search(expression.begin() + pos, expression.end(), token_match, token_regex, regex_constants::match_continuous))
          throw runtime_error("Unknown Expression Token");

        auto op = match_operator(token_match[0], nextop);

        if (static_cast<int>(op.code) != 255)
        {
          while (true)
          {
            if (nextop == OpType::PrefixOp)
            {
              operatorstack.push(op);

              break;
            }

            if (operatorstack.size() == 0 || operatorstack.top().precedence > op.precedence)
            {
              operatorstack.push(op);

              break;
            }

            if (static_cast<int>(operatorstack.top().code) == 252) // '('
            {
              if (static_cast<int>(op.code) != 254) // ','
                operatorstack.pop();

              break;
            }

            result.push(operatorstack.top().code);

            operatorstack.pop();
          }

          if (static_cast<int>(op.code) != 253) // ')'
            nextop = OpType::PrefixOp;
        }
        else
        {
          if (constants.find(token_match[0]) != constants.end())
            result.push(constants.find(token_match[0])->second);

          else if (token_match[0] == "true")
            result.push(1.0f);

          else if (token_match[0] == "false")
            result.push(0.0f);

          else if (token_match[1].matched)
            result.push(stof(token_match[0]));

          else
            result.push(token_match[0]);

          nextop = OpType::InfixOp;
        }

        pos += token_match[0].length();
      }

      while (operatorstack.size() > 0)
      {
        result.push(operatorstack.top().code);

        operatorstack.pop();
      }

      return result;
    }


    ///////////////////////// find_item ///////////////////////////////////////

    struct ItemReference
    {
      Ui::Id id;
      int16_t index;
    };

    ItemReference find_item(string variable)
    {
      int16_t index = 0;

      while (variable.substr(0, 7) == "parent.")
      {
        index -= 1;
        variable = variable.substr(7);
      }

      if (size_t(1 - index) > itemstack.size())
        throw runtime_error("Unknown Parent");

      auto type = itemstack[itemstack.size() + index - 1];

      for(auto &item : itemlist)
      {
        if (variable.substr(0, item.first.size()) == item.first && variable[item.first.size()] == '.')
        {
          type = item.second;
          index = indexof(itemlist, item) + 1;
          variable = variable.substr(item.first.size() + 1);
        }
      }

      return { parse_id(type + "." + variable), index };
    }


    ///////////////////////// bytecode ////////////////////////////////////////

    void push_createitem(vector<uint8_t> &bytecode, Ui::Id id)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::CreateItem);
      bytecode.insert(bytecode.end(), (uint8_t*)&id, (uint8_t*)&id + sizeof(id));
    }

    void push_setid(vector<uint8_t> &bytecode, string const &id)
    {
      uint32_t size = uint32_t(id.length());
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::SetId);
      bytecode.insert(bytecode.end(), (uint8_t*)&size, (uint8_t*)&size + sizeof(size));
      bytecode.insert(bytecode.end(), (uint8_t const *)id.data(), (uint8_t const*)id.data() + id.size());
    }

    void push_storeint(vector<uint8_t> &bytecode, Ui::Id id, int32_t value)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::StoreInt);
      bytecode.insert(bytecode.end(), (uint8_t*)&id, (uint8_t*)&id + sizeof(id));
      bytecode.insert(bytecode.end(), (uint8_t*)&value, (uint8_t*)&value + sizeof(value));
    }

    void push_storebool(vector<uint8_t> &bytecode, Ui::Id id, int32_t value)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::StoreBool);
      bytecode.insert(bytecode.end(), (uint8_t*)&id, (uint8_t*)&id + sizeof(id));
      bytecode.insert(bytecode.end(), (uint8_t*)&value, (uint8_t*)&value + sizeof(value));
    }

    void push_storefloat(vector<uint8_t> &bytecode, Ui::Id id, float value)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::StoreFloat);
      bytecode.insert(bytecode.end(), (uint8_t*)&id, (uint8_t*)&id + sizeof(id));
      bytecode.insert(bytecode.end(), (uint8_t*)&value, (uint8_t*)&value + sizeof(value));
    }

    void push_storecolor(vector<uint8_t> &bytecode, Ui::Id id, Color4 const &value)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::StoreColor);
      bytecode.insert(bytecode.end(), (uint8_t*)&id, (uint8_t*)&id + sizeof(id));
      bytecode.insert(bytecode.end(), (uint8_t const *)&value, (uint8_t const *)&value + sizeof(value));
    }

    void push_storetext(vector<uint8_t> &bytecode, Ui::Id id, string const &value)
    {
      uint32_t size = uint32_t(value.length());
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::StoreText);
      bytecode.insert(bytecode.end(), (uint8_t*)&id, (uint8_t*)&id + sizeof(id));
      bytecode.insert(bytecode.end(), (uint8_t*)&size, (uint8_t*)&size + sizeof(size));
      bytecode.insert(bytecode.end(), (uint8_t const *)value.data(), (uint8_t const *)value.data() + value.size());
    }

    void push_beginexpr(vector<uint8_t> &bytecode, Ui::Expression const &expr)
    {
      uint32_t size = uint32_t(expr.size());
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::BeginExpr);
      bytecode.insert(bytecode.end(), (uint8_t*)&size, (uint8_t*)&size + sizeof(size));
      bytecode.insert(bytecode.end(), (uint8_t const *)expr.data(), (uint8_t const *)expr.data() + expr.size());
    }

    void push_patchexpr(vector<uint8_t> &bytecode, uint32_t offset, ItemReference item)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::PatchExpr);
      bytecode.insert(bytecode.end(), (uint8_t*)&offset, (uint8_t*)&offset + sizeof(offset));
      bytecode.insert(bytecode.end(), (uint8_t*)&item.index, (uint8_t*)&item.index + sizeof(item.index));
      bytecode.insert(bytecode.end(), (uint8_t*)&item.id, (uint8_t*)&item.id + sizeof(item.id));
    }

    void push_storeexpr(vector<uint8_t> &bytecode, Ui::Id id)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::StoreExpr);
      bytecode.insert(bytecode.end(), (uint8_t*)&id, (uint8_t*)&id + sizeof(id));
    }

    void push_storeanim(vector<uint8_t> &bytecode, ItemReference target, int32_t flags, float duration, float startvalue, float finishvalue)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::StoreAnim);
      bytecode.insert(bytecode.end(), (uint8_t*)&target.index, (uint8_t*)&target.index + sizeof(target.index));
      bytecode.insert(bytecode.end(), (uint8_t*)&target.id, (uint8_t*)&target.id + sizeof(target.id));
      bytecode.insert(bytecode.end(), (uint8_t*)&flags, (uint8_t*)&flags + sizeof(flags));
      bytecode.insert(bytecode.end(), (uint8_t*)&duration, (uint8_t*)&duration + sizeof(duration));
      bytecode.insert(bytecode.end(), (uint8_t*)&startvalue, (uint8_t*)&startvalue + sizeof(startvalue));
      bytecode.insert(bytecode.end(), (uint8_t*)&finishvalue, (uint8_t*)&finishvalue + sizeof(finishvalue));
    }

    void push_pop(vector<uint8_t> &bytecode)
    {
      bytecode.insert(bytecode.end(), (uint8_t)Ui::Op::Pop);
    }


    ///////////////////////// parse ///////////////////////////////////////////
    void parse(vector<uint8_t> &bytecode, sapentry &entry)
    {
      if (entry["EntryType"] == "Animation")
      {
        parse_animation(bytecode, entry);
      }
      else
      {
        parse_item(bytecode, entry);

        push_pop(bytecode);

        itemstack.pop_back();
      }
    }


    ///////////////////////// parse_template //////////////////////////////////
    void parse_template(sapentry &entry)
    {
      auto sv = leap::string_view(entry["EntryId"]);

      Template templat;
      templat.name = trim(sv.substr(0, sv.find(':'))).to_string();
      templat.basetype = (sv.find(':') != leap::string_view::npos) ? trim(sv.substr(sv.find(':') + 1)).to_string() : templat.name;
      templat.entry = entry;

      templates.emplace(templat.name, templat);
    }


    ///////////////////////// parse_animation /////////////////////////////////
    void parse_animation(vector<uint8_t> &bytecode, sapentry &entry)
    {
      if (!entry.defined("target"))
        throw runtime_error("No Animation Target");

      auto target = find_item(entry["target"]);
      auto enabled = parse_expression(entry.defined("enabled") ? entry["enabled"] : "true");
      auto flags = 0;
      auto duration = parse_float_literal(entry.defined("duration") ? entry["duration"] : "0.250");
      auto startvalue = parse_float_literal(entry.defined("startvalue") ? entry["startvalue"] : "0.0");
      auto finishvalue = parse_float_literal(entry.defined("finishvalue") ? entry["finishvalue"] : "1.0");

      if (!entry.defined("startvalue") && !entry.defined("finishvalue"))
        flags |= Ui::Animation::Bound;

      if (parse_bool_literal(entry["repeat"]))
        flags |= Ui::Animation::Repeat;

      if (parse_bool_literal(entry["clip"]))
        flags |= Ui::Animation::Clip;

      if (parse_bool_literal(entry["visible"]))
        flags |= Ui::Animation::Visible;

      push_beginexpr(bytecode, enabled.bytecode);

      for(auto &patch : enabled.patchtable)
      {
        push_patchexpr(bytecode, patch.first, find_item(patch.second));
      }

      push_storeanim(bytecode, target, flags, duration, startvalue, finishvalue);
    }


    ///////////////////////// parse_item //////////////////////////////////////
    void parse_item(vector<uint8_t> &bytecode, sapentry &entry)
    {
      auto itemtype = entry["EntryType"];

      vector<Template const *> templatestack;

      for(auto i = templates.find(itemtype); i != templates.end(); i = templates.find(itemtype))
      {
        itemtype = i->second.basetype;

        size_t ip = 0;
        for(auto &attr : i->second.entry)
        {
          if (!entry.defined(attr.name))
            entry.insert(ip++, attr.name, attr.value);
        }

        templatestack.insert(templatestack.begin(), &i->second);

        if (i->second.name == i->second.basetype)
          break;
      }

      if (itemstack.size() != 0)
      {
        size_t ip = 0;

        if (!entry.defined("x"))
          entry.insert(ip++, "x", "0");

        if (!entry.defined("y"))
          entry.insert(ip++, "y", "0");

        if (!entry.defined("width"))
          entry.insert(ip++, "width", "parent.width");

        if (!entry.defined("height"))
          entry.insert(ip++, "height", "parent.height");

        if (!entry.defined("scale"))
          entry.insert(ip++, "scale", "1");

        if (!entry.defined("rotation"))
          entry.insert(ip++, "rotation", "0");

        if (!entry.defined("opacity"))
          entry.insert(ip++, "opacity", "1");
      }

      push_createitem(bytecode, parse_id(itemtype));

      itemstack.push_back(itemtype);

      for(auto &attr : entry)
      {
        if (attr.name == "EntryType" || attr.name == "EntryId")
          continue;

        if (attr.name == "id")
        {
          push_setid(bytecode, attr.value);

          itemlist.emplace_back(attr.value, itemtype);

          continue;
        }

        auto id = parse_id(itemtype + "." + attr.name);

        if (is_literal(attr.value))
        {
          switch (Ui::type_of(id))
          {
            case Ui::Type::Int:
              push_storeint(bytecode, id, parse_int_literal(attr.value));
              break;

            case Ui::Type::Bool:
              push_storebool(bytecode, id, parse_bool_literal(attr.value));
              break;

            case Ui::Type::Float:
              push_storefloat(bytecode, id, parse_float_literal(attr.value));
              break;

            case Ui::Type::Color:
              push_storecolor(bytecode, id, parse_color_literal(attr.value));
              break;

            case Ui::Type::Font:
            case Ui::Type::Sprite:
            case Ui::Type::Text:
              push_storetext(bytecode, id, parse_text_literal(attr.value));
              break;

            case Ui::Type::Item:
              throw runtime_error("Invalid Literal Type");
          }
        }
        else
        {
          std::vector<pair<Ui::Id, string>> expressions;

          if (attr.value.find('?') != string::npos && attr.value.find(':') != string::npos)
          {
            auto sv = leap::string_view(attr.value);
            auto cond = trim(sv.substr(0, sv.find('?'))).to_string();
            auto literal1 = trim(sv.substr(sv.find('?') + 1, sv.rfind(':') - sv.find('?') - 1)).to_string();
            auto literal2 = trim(sv.substr(sv.rfind(':') + 1)).to_string();

            switch (Ui::type_of(id))
            {
              case Ui::Type::Int:
              case Ui::Type::Bool:
              case Ui::Type::Float:
                expressions.emplace_back(id, "if(" + cond + ", " + literal1 + ", " + literal2 + ")");
                break;

              case Ui::Type::Color:
              {
                auto name = itemtype + "." + attr.name;

                auto color1 = parse_color_literal(literal1);
                auto color2 = parse_color_literal(literal2);

                expressions.emplace_back(parse_id(name + ".r"), "if(" + cond + ", " + toa(color1.r) + ", " + toa(color2.r) + ")");
                expressions.emplace_back(parse_id(name + ".g"), "if(" + cond + ", " + toa(color1.g) + ", " + toa(color2.g) + ")");
                expressions.emplace_back(parse_id(name + ".b"), "if(" + cond + ", " + toa(color1.b) + ", " + toa(color2.b) + ")");
                expressions.emplace_back(parse_id(name + ".a"), "if(" + cond + ", " + toa(color1.a) + ", " + toa(color2.a) + ")");

                break;
              }

              default:
                throw runtime_error("Invalid Expression Type");
            }
          }
          else
          {
            expressions.emplace_back(id, attr.value);
          }

          for(auto &expression : expressions)
          {
            auto id = expression.first;
            auto expr = parse_expression(expression.second);

            push_beginexpr(bytecode, expr.bytecode);

            for(auto &patch : expr.patchtable)
            {
              push_patchexpr(bytecode, patch.first, find_item(patch.second));
            }

            push_storeexpr(bytecode, id);
          }
        }
      }

      sapentry childentry;

      for(auto &templat : templatestack)
      {
        auto substream = templat->entry.substream();

        while (substream >> childentry)
        {
          parse(bytecode, childentry);
        }
      }

      while (entry.substream() >> childentry)
      {
        parse(bytecode, childentry);
      }
    }

    vector<uint8_t> parse(string const &src)
    {
      vector<uint8_t> bytecode;

      issapstream stream(src);

      sapentry entry;

      while (stream >> entry)
      {
        if (entry["EntryType"] == "template")
        {
          parse_template(entry);

          continue;
        }

        parse_item(bytecode, entry);

        break;
      }

      if (itemstack.size() != 1)
        messages.emplace_back("TopLevel Item not found");

      if (stream >> entry)
        messages.emplace_back("Unsupported TopLevel Item found");

      return bytecode;
    }

    public:

      vector<string> messages;

      struct Template
      {
        string name;
        string basetype;

        sapentry entry;
      };

      map<string, Template> templates;

    private:

      vector<string> itemstack;

      vector<pair<string, string>> itemlist;

      unordered_map<string, Ui::Id> ids;
      unordered_map<string, int> constants;
  };
}


///////////////////////// load_ui ///////////////////////////////////////////
string load_ui(pathstring const &path, string const &defines)
{
  ifstream fin(path);

  if (!fin)
    throw runtime_error("Error opening - " + path.name());

  int line = 1;

  string ui = "#line " + to_string(line) + "\"" + path.name() + "\"\n";

  string buffer;

  while (getline(fin, buffer))
  {
    ++line;

    if (buffer.substr(0, 8) == "#include")
    {
      buffer = load_ui(pathstring(path.base(), string(buffer.begin() + buffer.find_first_of('"') + 1, buffer.begin() + buffer.find_last_of('"'))));

      buffer += "\n#line " + to_string(line) + "\"" + path.name() + "\"";
    }

    ui += buffer + '\n';
  }

  return ui;
}


///////////////////////// parse_ui //////////////////////////////////////////
vector<uint8_t> parse_ui(string const &src)
{
  Parser parser;

  auto bytecode = parser.parse(src);

  for(auto &message : parser.messages)
    cout << "Parser: " << message << endl;

  return bytecode;
}
