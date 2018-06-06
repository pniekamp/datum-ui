//
// datum-ui - reflect generator
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace std;
  
struct Field
{
  string name;
  string type;
};

struct Item
{
  string name;
  string parent;
  vector<Field> fields;
  vector<string> constants;
};

vector<Item> items;
int itementropy[256];

vector<string> types;

void dump_id_enum()
{
  cout << "  enum class Id : uint16_t" << '\n';
  cout << "  {" << '\n';
  
  for(auto &item : items)
  {
    auto itemid = itementropy[&item - &items.front()];
    
    cout << "    " << item.name << " = 0x" << hex << setw(2) << setfill('0') << itemid << dec << setw(0) << "00," << '\n';

    int fieldid = 0;
    int fieldentropy[256];
    iota(begin(fieldentropy), end(fieldentropy), 0);
    random_shuffle(begin(fieldentropy), end(fieldentropy));

    for(auto &field : item.fields)
    {
      cout << "    " << item.name << "_" << field.name << " = 0x" << hex << setw(2) << setfill('0') << itemid << fieldentropy[fieldid++] << dec << setw(0) << "," << '\n';
      
      if (field.type == "Color")
      {
        cout << "    " << item.name << "_" << field.name << "_r = 0x" << hex << setw(2) << setfill('0') << itemid << fieldentropy[fieldid++] << dec << setw(0) << "," << '\n';
        cout << "    " << item.name << "_" << field.name << "_g = 0x" << hex << setw(2) << setfill('0') << itemid << fieldentropy[fieldid++] << dec << setw(0) << "," << '\n';
        cout << "    " << item.name << "_" << field.name << "_b = 0x" << hex << setw(2) << setfill('0') << itemid << fieldentropy[fieldid++] << dec << setw(0) << "," << '\n';
        cout << "    " << item.name << "_" << field.name << "_a = 0x" << hex << setw(2) << setfill('0') << itemid << fieldentropy[fieldid++] << dec << setw(0) << "," << '\n';
      }
    }
    
    cout << '\n';
  }
  
  cout << "  };" << '\n';
  cout << '\n';
}

void dump_type_enum()
{
  cout << "  enum class Type : uint8_t" << '\n';
  cout << "  {" << '\n';
  
  for(auto &type : types)
  {
    cout << "    " << type << "," << '\n';
  }

  cout << "  };" << '\n';
  cout << '\n';
}

void dump_op_enum()
{
  cout << "  enum class Op : uint8_t" << '\n';
  cout << "  {" << '\n';
  cout << "    CreateItem = 0x9f," << '\n';
  cout << "    SetId = 0x6a," << '\n';
  cout << "    StoreInt = 0x2e," << '\n';
  cout << "    StoreBool = 0xa4," << '\n';
  cout << "    StoreFloat = 0xb6," << '\n';
  cout << "    StoreColor = 0x9d," << '\n';
  cout << "    StoreText = 0xb1," << '\n';
  cout << "    BeginExpr = 0x94," << '\n';
  cout << "    PatchExpr = 0xb8," << '\n';
  cout << "    StoreExpr = 0x64," << '\n';
  cout << "    StoreAnim = 0x35," << '\n';
  cout << "    Pop = 0xc7," << '\n';
  cout << "  };" << '\n';
  cout << '\n';
}

void dump_type_function()
{
  cout << "    ///////////////////////// type_of //////////////////////////////////////////" << '\n';
  cout << "    [[gnu::unused]] Type type_of(Id id)" << '\n';
  cout << "    {" << '\n';
  cout << "      switch(id)" << '\n';
  cout << "      {" << '\n';
  
  for(auto &type : types)
  {
    for(auto &item : items)
    {
      if (type == "Item")
      {
        cout << "        case Id::" << item.name << ":" << '\n';
      }
      
      for(auto &field: item.fields)
      {
        if (field.type == type)
        {
          cout << "        case Id::" << item.name << "_" << field.name << ":" << '\n';
        }

        if (field.type == "Color" && type == "Float")
        {
          cout << "        case Id::" << item.name << "_" << field.name << "_r" << ":" << '\n';
          cout << "        case Id::" << item.name << "_" << field.name << "_g" << ":" << '\n';
          cout << "        case Id::" << item.name << "_" << field.name << "_b" << ":" << '\n';
          cout << "        case Id::" << item.name << "_" << field.name << "_a" << ":" << '\n';
        }
      }
    }

    cout << "          return Type::" << type << ";" << endl << '\n';
  }

  cout << "      }" << '\n';
  cout << '\n';
  cout << "      throw std::runtime_error(\"Unknown Type\");" << '\n';
  cout << "    }" << '\n';
}

void dump_address_function()
{
  cout << "    ///////////////////////// address_of //////////////////////////////////////////" << '\n';
  cout << '\n';

  for(auto &type : types)
  {
    if (type == "Item")
      continue;

    auto atype = type;
    for(auto &ch : atype)
      ch = tolower(ch);

    auto rtype = type + " *";
    if (type == "Int" || type == "Bool" || type == "Float")
      rtype = atype + " *";
    if (type == "Color")
      rtype = "Color4 *";
    if (type == "Text")
      rtype = "const char **";
  
    cout << "    [[gnu::unused]] " << rtype << "address_of_" << atype << "(Item *item, Id id)" << '\n';
    cout << "    {" << '\n';
    cout << "      switch(id)" << '\n';
    cout << "      {" << '\n';
  
    for(auto &item : items)
    {      
      for(auto &field: item.fields)
      {
        if (field.type == type)
        {
          cout << "        case Id::" << item.name << "_" << field.name << ":" << '\n';
          cout << "          return &static_cast<" << item.name << "*>(item)->" << field.name << ";" << endl << '\n';
        }

        if (field.type == "Color" && type == "Float")
        {
          cout << "        case Id::" << item.name << "_" << field.name << "_r" << ":" << '\n';
          cout << "          return &static_cast<" << item.name << "*>(item)->" << field.name << ".r" << ";" << endl << '\n';
          cout << "        case Id::" << item.name << "_" << field.name << "_g"<< ":" << '\n';
          cout << "          return &static_cast<" << item.name << "*>(item)->" << field.name << ".g" << ";" << endl << '\n';
          cout << "        case Id::" << item.name << "_" << field.name << "_b"<< ":" << '\n';
          cout << "          return &static_cast<" << item.name << "*>(item)->" << field.name << ".b" << ";" << endl << '\n';
          cout << "        case Id::" << item.name << "_" << field.name << "_a"<< ":" << '\n';
          cout << "          return &static_cast<" << item.name << "*>(item)->" << field.name << ".a" << ";" << endl << '\n';
        }
      }
    }

    cout << "        default:" << '\n';
    cout << "          throw std::runtime_error(\"Invalid " << type << " Id\");" << '\n';

    cout << "      }" << '\n';
    cout << "    }" << '\n';
    cout << '\n';
  }
}

void dump_alwaysbind_function()
{
  cout << "    ///////////////////////// always_bind /////////////////////////////////////" << '\n';
  cout << "    [[gnu::unused]] bool always_bind(Id id)" << '\n';
  cout << "    {" << '\n';
  cout << "      switch (id)" << '\n';
  cout << "      {" << '\n';
  cout << "        case Ui::Id::Item_x:" << '\n';
  cout << "        case Ui::Id::Item_y:" << '\n';
  cout << "        case Ui::Id::Item_width:" << '\n';
  cout << "        case Ui::Id::Item_height:" << '\n';
  cout << "        case Ui::Id::Item_scale:" << '\n';
  cout << "        case Ui::Id::Item_rotation:" << '\n';
  cout << "        case Ui::Id::Item_opacity:" << '\n';
  cout << "        case Ui::Id::Control_fontheight:" << '\n';
  cout << "        case Ui::Id::ScrollArea_contentwidth:" << '\n';
  cout << "        case Ui::Id::ScrollArea_contentheight:" << '\n';
  cout << "          return true;" << '\n';
  cout << '\n';
  cout << "        default:" << '\n';
  cout << "          return false;" << '\n';
  cout << "      }" << '\n';
  cout << "    }" << '\n';
  cout << '\n';
}

void dump_create_function()
{
    cout << "    ///////////////////////// create_item /////////////////////////////////////" << '\n';
    cout << "    [[gnu::unused]] Item *create_item(Context &context, Id id, Item *parent = nullptr)" << '\n';
    cout << "    {" << '\n';
    cout << "      switch (id)" << '\n';
    cout << "      {" << '\n';

    for(auto &item : items)
    {
      if (item.name == "Control")
        continue;

      cout << "        case Ui::Id::" << item.name << ":" << '\n';
      cout << "          return context.create_item<Ui::" << item.name << ">(parent);" << endl << '\n';
    }
    
    cout << "        default:" << '\n';
    cout << "          throw std::runtime_error(\"Invalid Item Id\");" << '\n';
    cout << "      }" << '\n';
    cout << "    }" << '\n';
    cout << '\n';
}    

void dump_name_function()
{
  cout << "    ///////////////////////// name_of //////////////////////////////////////////" << '\n';
  cout << "    [[gnu::unused]] const char *name_of(Id id)" << '\n';
  cout << "    {" << '\n';
  cout << "      switch(id)" << '\n';
  cout << "      {" << '\n';

  for(auto &item : items)
  {
    cout << "        case Id::" << item.name << ":" << '\n';
    cout << "          return \"" << item.name << "\";" << '\n';

    for(auto &field: item.fields)
    {
      cout << "        case Id::" << item.name << "_" << field.name << ":" << '\n';
      cout << "          return \"" << field.name << "\";" << '\n';

      if (field.type == "Color")
      {
        cout << "        case Id::" << item.name << "_" << field.name << "_r" << ":" << '\n';
        cout << "          return \"" << field.name << ".r\";" << '\n';
        cout << "        case Id::" << item.name << "_" << field.name << "_g" << ":" << '\n';
        cout << "          return \"" << field.name << ".g\";" << '\n';
        cout << "        case Id::" << item.name << "_" << field.name << "_b" << ":" << '\n';
        cout << "          return \"" << field.name << ".b\";" << '\n';
        cout << "        case Id::" << item.name << "_" << field.name << "_a" << ":" << '\n';
        cout << "          return \"" << field.name << ".a\";" << '\n';
      }
    }
  }

  cout << "      }" << '\n';
  cout << '\n';
  cout << "      throw std::runtime_error(\"Unknown Type\");" << '\n';
  cout << "    }" << '\n';
  cout << '\n';
}


void dump_idmap_function()
{
  cout << "    ///////////////////////// init_ids_map ////////////////////////////////////" << '\n';
  cout << "    [[gnu::unused]] void init_ids_map(std::unordered_map<std::string, Id> &ids)" << '\n';
  cout << "    {" << '\n';
  
  for(auto &item : items)
  {
    cout << "      ids[\"" << item.name << "\"] = Id::" << item.name << ";" << '\n';
    
    vector<Item*> itemstack;    
    for(auto name = item.name; name != ""; )
    {
      auto j = find_if(items.begin(), items.end(), [&](auto &item) { return (item.name == name); });
      
      if (j == items.end())
        break;
        
      itemstack.insert(itemstack.begin(), &*j);
      
      name = j->parent; 
    }
    
    for(auto &hitem : itemstack)
    {
      for(auto &field : hitem->fields)
      {
        cout << "      ids[\"" << item.name << "." << field.name << "\"] = Id::" << hitem->name << "_" << field.name << ";" << '\n';
        
        if (field.type == "Color")
        {
          cout << "      ids[\"" << item.name << "." << field.name << ".r" << "\"] = Id::" << hitem->name << "_" << field.name << "_r" << ";" << '\n';
          cout << "      ids[\"" << item.name << "." << field.name << ".g" << "\"] = Id::" << hitem->name << "_" << field.name << "_g" << ";" << '\n';
          cout << "      ids[\"" << item.name << "." << field.name << ".b" << "\"] = Id::" << hitem->name << "_" << field.name << "_b" << ";" << '\n';
          cout << "      ids[\"" << item.name << "." << field.name << ".a" << "\"] = Id::" << hitem->name << "_" << field.name << "_a" << ";" << '\n';
        }        
      }
    }    
  }
  
  cout << "    }" << '\n';
  cout << '\n';
}

void dump_constantmap_function()
{
  cout << "    ///////////////////////// init_constants_map ////////////////////////////" << '\n';
  cout << "    [[gnu::unused]] void init_constants_map(std::unordered_map<std::string, int> &constants)" << '\n';
  cout << "    {" << '\n';
  
  for(auto &item : items)
  {
    for(auto &constant : item.constants)
    {
      cout << "      constants[\"" << item.name << "." << constant << "\"] = " << item.name << "::" << constant << ";" << '\n';
    }
    
    if (item.constants.size() != 0)
      cout << '\n';
  }
  
  cout << "    }" << '\n';
  cout << '\n';
}

int main()
{
  types = { "Int", "Bool", "Float", "Color", "Font", "Sprite", "Text", "Item" };
  
  items.push_back({ "Item", "", { { "x", "Float" }, { "y", "Float" }, { "width", "Float" }, { "height", "Float" }, { "z", "Float" }, { "scale", "Float" }, { "rotation", "Float" }, { "opacity", "Float" }, { "clip", "Bool" }, { "visible", "Bool" } } });
  items.push_back({ "Rectangle", "Item", { { "color", "Color" }, { "borderwidth", "Int" }, { "bordercolor", "Color" } } });
  items.push_back({ "Text", "Item", { { "text", "Text" }, { "font", "Font" }, { "color", "Color" }, { "horizontalalign", "Int" }, { "verticalalign", "Int" } }, { "Left", "Center", "Right", "Justified", "Top", "Middle", "Bottom", "BaseLine" } });
  items.push_back({ "Image", "Item", { { "image", "Sprite" }, { "tint", "Color" }, { "layer", "Float" }, { "fillmode", "Int" }, { "horizontalalign", "Int" }, { "verticalalign", "Int" } }, { "Stretch", "Fit", "Crop", "Pad", "Left", "Center", "Right", "Top", "Middle", "Bottom" } });
  items.push_back({ "Frame", "Item", { { "image", "Sprite" }, { "tint", "Color" }, { "layer", "Float" }, { "slice", "Float" } } });
  items.push_back({ "Row", "Item", { { "spacing", "Float" } } });
  items.push_back({ "Column", "Item", { { "spacing", "Float" } } });
  items.push_back({ "ScrollBar", "Item", { { "value", "Float" }, { "size", "Float" }, { "orientation", "Int" }, { "handleimage", "Sprite" }, { "handletint", "Color" }, { "handlelayer", "Float" }, { "handleslice", "Float" }, { "handleinset", "Float" }, { "hovered", "Bool" }, { "pressed", "Bool" } }, { "Horizontal", "Vertical" } });
  items.push_back({ "ScrollArea", "Item", { { "contentwidth", "Float" }, { "contentheight", "Float" }, { "handleimage", "Sprite" }, { "handletint", "Color" }, { "handlelayer", "Float" }, { "handleslice", "Float" }, { "handlesize", "Float" }, { "handleinset", "Float" }, { "wheelscroll", "Bool" }, { "interactive", "Bool" } } });
  items.push_back({ "TextArea", "Item", { { "text", "Text" }, { "font", "Font" }, { "textcolor", "Color" }, { "horizontalalign", "Int" }, { "verticalalign", "Int" }, { "selectioncolor", "Color" }, { "selectiontextcolor", "Color" }, { "interactive", "Bool" } }, { "Left", "Center", "Right", "Justified", "Top", "Middle", "Bottom" } });
  items.push_back({ "Control", "Item", { { "borderwidth", "Int" }, { "bordercolor", "Color" }, { "backgroundimage", "Sprite" }, { "backgroundcolor", "Color" }, { "backgroundlayer", "Float" }, { "backgroundslice", "Float" }, { "font", "Font" }, { "fontheight", "Float" }, { "enabled", "Bool" }, { "hovered", "Bool" }, { "action", "Int" } } });
  items.push_back({ "Label", "Control", { { "text", "Text" }, { "textcolor", "Color" }, { "horizontalalign", "Int" }, { "verticalalign", "Int" }, { "padding", "Float" } }, { "Left", "Center", "Right", "Justified", "Top", "Middle", "Bottom", "BaseLine" } });
  items.push_back({ "Field", "Control", { { "value", "Float" }, { "precision", "Int" }, { "format", "Text" }, { "textcolor", "Color" }, { "horizontalalign", "Int" }, { "verticalalign", "Int" }, { "padding", "Float" } }, { "Left", "Center", "Right", "Top", "Middle", "Bottom", "BaseLine" } });
  items.push_back({ "Button", "Control", { { "text", "Text" }, { "textcolor", "Color" }, { "textalign", "Int" }, { "image", "Sprite" }, { "imagetint", "Color" }, { "imagelayer", "Float" }, { "imagealign", "Int" }, { "spacing", "Float" }, { "padding", "Float" }, { "pressed", "Bool" } }, { "Left", "Center", "Right", "Before", "Fill", "After", "Above", "Below" } });
  items.push_back({ "CheckButton", "Button", { { "checked", "Int" }, { "innercolor", "Color" }, { "outercolor", "Color" } }, { "Left", "Center", "Right", "Before", "Fill", "After", "Above", "Below", "UnChecked", "Indeterminate", "Checked" } });
  items.push_back({ "DragButton", "Button", { { "dx", "Float" }, { "dy", "Float" } } });
  items.push_back({ "Slider", "Control", { { "value", "Float" }, { "minvalue", "Float" }, { "maxvalue", "Float" }, { "orientation", "Int" }, { "inset", "Float" }, { "handleimage", "Sprite" }, { "handletint", "Color" }, { "handlelayer", "Float" }, { "pressed", "Bool" } }, { "Horizontal", "Vertical" } });
  items.push_back({ "LineEdit", "Control", { { "text", "Text" }, { "textcolor", "Color" }, { "padding", "Float" }, { "selectioncolor", "Color" }, { "selectiontextcolor", "Color" }, { "focused", "Bool" }, { "filter", "Int"} }, { "Readonly", "Integer", "Decimal", "Text" } });
  items.push_back({ "SlideEdit", "LineEdit", { { "value", "Float" }, { "precision", "Int" }, { "format", "Text" }, { "minvalue", "Float" }, { "maxvalue", "Float" }, { "handleimage", "Sprite" }, { "handleimagetint", "Color" }, { "handleimagelayer", "Float" }, { "sliderate", "Float" } } });
  items.push_back({ "ListBox", "Control", { { "text", "Text" }, { "textcolor", "Color" }, { "handleimage", "Sprite" }, { "handletint", "Color" }, { "handlelayer", "Float" }, { "handleslice", "Float" }, { "handlesize", "Float" }, { "handleinset", "Float" }, { "padding", "Float" }, { "selectioncolor", "Color" }, { "selectiontextcolor", "Color" }, { "focused", "Bool" } } });
  items.push_back({ "ComboBox", "Control", { { "text", "Text" }, { "textcolor", "Color" }, { "dropimage", "Sprite" }, { "dropimagetint", "Color" }, { "dropimagelayer", "Float" }, { "listborderwidth", "Int" }, { "listbordercolor", "Color" }, { "listbackgroundcolor", "Color" }, { "handleimage", "Sprite" }, { "handletint", "Color" }, { "handlelayer", "Float" }, { "handleslice", "Float" }, { "handlesize", "Float" }, { "handleinset", "Float" }, { "padding", "Float" }, { "selectioncolor", "Color" }, { "selectiontextcolor", "Color" }, { "pressed", "Bool" }, { "dropped", "Bool" } } });
  items.push_back({ "GroupBox", "CheckButton", { }, { "Left", "Center", "Right", "Before", "Fill", "After", "Above", "Below", "UnChecked", "Indeterminate", "Checked" } });

  iota(begin(itementropy), end(itementropy), 0);
  random_shuffle(begin(itementropy), end(itementropy));

  cout << "//" << '\n';
  cout << "// datum-ui - compiler reflection data" << '\n';
  cout << "//" << '\n';
  cout << '\n';
  cout << "#pragma once" << '\n';
  cout << '\n';
  cout << "#include \"items.h\"" << '\n';
  cout << "#include \"scrollarea.h\"" << '\n';
  cout << "#include \"textarea.h\"" << '\n';
  cout << "#include \"label.h\"" << '\n';
  cout << "#include \"button.h\"" << '\n';
  cout << "#include \"slider.h\"" << '\n';
  cout << "#include \"lineedit.h\"" << '\n';
  cout << "#include \"slideedit.h\"" << '\n';
  cout << "#include \"listbox.h\"" << '\n';
  cout << "#include \"combobox.h\"" << '\n';
  cout << "#include \"groupbox.h\"" << '\n';
  cout << "#include <unordered_map>" << '\n';
  cout << "#include <stdexcept>" << '\n';
  cout << '\n';
  cout << "namespace Ui" << '\n';
  cout << "{" << '\n';

  dump_id_enum();
  dump_type_enum();
  dump_op_enum(); 

  cout << "  namespace" << '\n';
  cout << "  {" << '\n';
  
  dump_type_function();
  dump_address_function();
  dump_alwaysbind_function();
  dump_create_function();

  cout << "#ifdef DATUMUIC" << '\n';

  dump_name_function();
  dump_idmap_function();
  dump_constantmap_function();

  cout << "#endif" << '\n';

  cout << "  }" << '\n';

  cout << "}" << '\n';
  
  return 0;
}
