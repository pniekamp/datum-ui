//
// datum-ui - compiler reflection data
//

#pragma once

#include "items.h"
#include "scrollarea.h"
#include "textarea.h"
#include "label.h"
#include "button.h"
#include "slider.h"
#include "lineedit.h"
#include "slideedit.h"
#include "listbox.h"
#include "combobox.h"
#include "groupbox.h"
#include <unordered_map>
#include <stdexcept>

namespace Ui
{
  enum class Id : uint16_t
  {
    Item = 0xcd00,
    Item_x = 0xcd25,
    Item_y = 0xcdde,
    Item_width = 0xcd5a,
    Item_height = 0xcd72,
    Item_z = 0xcdbb,
    Item_scale = 0xcd32,
    Item_rotation = 0xcd93,
    Item_opacity = 0xcd53,
    Item_clip = 0xcd0,
    Item_visible = 0xcde6,

    Rectangle = 0xf000,
    Rectangle_color = 0xf026,
    Rectangle_color_r = 0xf05c,
    Rectangle_color_g = 0xf073,
    Rectangle_color_b = 0xf01e,
    Rectangle_color_a = 0xf034,
    Rectangle_borderwidth = 0xf0cb,
    Rectangle_bordercolor = 0xf00,
    Rectangle_bordercolor_r = 0xf0f0,
    Rectangle_bordercolor_g = 0xf0a3,
    Rectangle_bordercolor_b = 0xf07a,
    Rectangle_bordercolor_a = 0xf01c,

    Text = 0xa200,
    Text_text = 0xa2bd,
    Text_font = 0xa2dd,
    Text_color = 0xa26d,
    Text_color_r = 0xa2a0,
    Text_color_g = 0xa24a,
    Text_color_b = 0xa278,
    Text_color_a = 0xa2bf,
    Text_horizontalalign = 0xa25a,
    Text_verticalalign = 0xa20,

    Image = 0xe200,
    Image_image = 0xe262,
    Image_tint = 0xe2fa,
    Image_tint_r = 0xe2ee,
    Image_tint_g = 0xe2f,
    Image_tint_b = 0xe21d,
    Image_tint_a = 0xe2a0,
    Image_layer = 0xe260,
    Image_fillmode = 0xe2b7,
    Image_horizontalalign = 0xe24e,
    Image_verticalalign = 0xe2a8,

    Frame = 0xed00,
    Frame_image = 0xedb9,
    Frame_tint = 0xed26,
    Frame_tint_r = 0xedec,
    Frame_tint_g = 0xed68,
    Frame_tint_b = 0xed65,
    Frame_tint_a = 0xed19,
    Frame_layer = 0xed89,
    Frame_slice = 0xed52,

    Row = 0x8800,
    Row_spacing = 0x88f6,

    Column = 0xe300,
    Column_spacing = 0xe3e8,

    ScrollBar = 0x9800,
    ScrollBar_value = 0x9820,
    ScrollBar_size = 0x981c,
    ScrollBar_orientation = 0x9879,
    ScrollBar_handleimage = 0x98a0,
    ScrollBar_handletint = 0x98ed,
    ScrollBar_handletint_r = 0x9813,
    ScrollBar_handletint_g = 0x98b7,
    ScrollBar_handletint_b = 0x98fa,
    ScrollBar_handletint_a = 0x98ec,
    ScrollBar_handlelayer = 0x98aa,
    ScrollBar_handleslice = 0x9881,
    ScrollBar_handleinset = 0x986b,
    ScrollBar_hovered = 0x98f2,
    ScrollBar_pressed = 0x98b6,

    ScrollArea = 0xbc00,
    ScrollArea_contentwidth = 0xbca8,
    ScrollArea_contentheight = 0xbc28,
    ScrollArea_handleimage = 0xbc26,
    ScrollArea_handletint = 0xbc55,
    ScrollArea_handletint_r = 0xbcb6,
    ScrollArea_handletint_g = 0xbc53,
    ScrollArea_handletint_b = 0xbcc8,
    ScrollArea_handletint_a = 0xbc7d,
    ScrollArea_handlelayer = 0xbc34,
    ScrollArea_handleslice = 0xbc66,
    ScrollArea_handlesize = 0xbc6f,
    ScrollArea_handleinset = 0xbcba,
    ScrollArea_wheelscroll = 0xbcc7,
    ScrollArea_interactive = 0xbcfd,

    TextArea = 0x6600,
    TextArea_text = 0x668a,
    TextArea_font = 0x667b,
    TextArea_textcolor = 0x6632,
    TextArea_textcolor_r = 0x66d1,
    TextArea_textcolor_g = 0x6639,
    TextArea_textcolor_b = 0x666a,
    TextArea_textcolor_a = 0x6656,
    TextArea_horizontalalign = 0x66f,
    TextArea_verticalalign = 0x664c,
    TextArea_selectioncolor = 0x663d,
    TextArea_selectioncolor_r = 0x6672,
    TextArea_selectioncolor_g = 0x66f7,
    TextArea_selectioncolor_b = 0x661b,
    TextArea_selectioncolor_a = 0x6658,
    TextArea_selectiontextcolor = 0x667d,
    TextArea_selectiontextcolor_r = 0x66d3,
    TextArea_selectiontextcolor_g = 0x6662,
    TextArea_selectiontextcolor_b = 0x669c,
    TextArea_selectiontextcolor_a = 0x665e,
    TextArea_interactive = 0x661a,

    Control = 0xc100,
    Control_borderwidth = 0xc132,
    Control_bordercolor = 0xc197,
    Control_bordercolor_r = 0xc1ca,
    Control_bordercolor_g = 0xc1a2,
    Control_bordercolor_b = 0xc168,
    Control_bordercolor_a = 0xc112,
    Control_backgroundimage = 0xc1ae,
    Control_backgroundcolor = 0xc129,
    Control_backgroundcolor_r = 0xc1c4,
    Control_backgroundcolor_g = 0xc134,
    Control_backgroundcolor_b = 0xc1c9,
    Control_backgroundcolor_a = 0xc16d,
    Control_backgroundlayer = 0xc1b9,
    Control_backgroundslice = 0xc15d,
    Control_font = 0xc155,
    Control_fontheight = 0xc138,
    Control_enabled = 0xc164,
    Control_hovered = 0xc1fa,
    Control_action = 0xc12d,

    Label = 0x7c00,
    Label_text = 0x7cf,
    Label_textcolor = 0x7c19,
    Label_textcolor_r = 0x7ccb,
    Label_textcolor_g = 0x7c25,
    Label_textcolor_b = 0x7c58,
    Label_textcolor_a = 0x7c94,
    Label_horizontalalign = 0x7c30,
    Label_verticalalign = 0x7cbb,
    Label_padding = 0x7c32,

    Field = 0xf400,
    Field_value = 0xf486,
    Field_precision = 0xf494,
    Field_format = 0xf4d,
    Field_textcolor = 0xf44d,
    Field_textcolor_r = 0xf468,
    Field_textcolor_g = 0xf48f,
    Field_textcolor_b = 0xf4d1,
    Field_textcolor_a = 0xf496,
    Field_horizontalalign = 0xf498,
    Field_verticalalign = 0xf4c2,
    Field_padding = 0xf4a5,

    Button = 0xdb00,
    Button_text = 0xdbdd,
    Button_textcolor = 0xdb7b,
    Button_textcolor_r = 0xdb17,
    Button_textcolor_g = 0xdb6f,
    Button_textcolor_b = 0xdbb0,
    Button_textcolor_a = 0xdb42,
    Button_textalign = 0xdbda,
    Button_image = 0xdb8e,
    Button_imagetint = 0xdba6,
    Button_imagetint_r = 0xdb19,
    Button_imagetint_g = 0xdb6e,
    Button_imagetint_b = 0xdbc7,
    Button_imagetint_a = 0xdb98,
    Button_imagelayer = 0xdb68,
    Button_imagealign = 0xdb5c,
    Button_spacing = 0xdb7,
    Button_padding = 0xdbef,
    Button_pressed = 0xdb86,

    CheckButton = 0x4700,
    CheckButton_checked = 0x47e,
    CheckButton_innercolor = 0x47d8,
    CheckButton_innercolor_r = 0x47ad,
    CheckButton_innercolor_g = 0x4730,
    CheckButton_innercolor_b = 0x471d,
    CheckButton_innercolor_a = 0x47a3,
    CheckButton_outercolor = 0x477e,
    CheckButton_outercolor_r = 0x47de,
    CheckButton_outercolor_g = 0x4753,
    CheckButton_outercolor_b = 0x47f4,
    CheckButton_outercolor_a = 0x4790,

    DragButton = 0x9600,
    DragButton_dx = 0x96bc,
    DragButton_dy = 0x962b,

    Slider = 0xf600,
    Slider_value = 0xf6bf,
    Slider_minvalue = 0xf64c,
    Slider_maxvalue = 0xf6cd,
    Slider_orientation = 0xf651,
    Slider_inset = 0xf6b1,
    Slider_handleimage = 0xf674,
    Slider_handletint = 0xf677,
    Slider_handletint_r = 0xf667,
    Slider_handletint_g = 0xf670,
    Slider_handletint_b = 0xf669,
    Slider_handletint_a = 0xf63f,
    Slider_handlelayer = 0xf626,
    Slider_pressed = 0xf6a9,

    LineEdit = 0x6c00,
    LineEdit_text = 0x6c2f,
    LineEdit_textcolor = 0x6c89,
    LineEdit_textcolor_r = 0x6cca,
    LineEdit_textcolor_g = 0x6c50,
    LineEdit_textcolor_b = 0x6ce3,
    LineEdit_textcolor_a = 0x6c1c,
    LineEdit_padding = 0x6c3a,
    LineEdit_selectioncolor = 0x6cf,
    LineEdit_selectioncolor_r = 0x6c32,
    LineEdit_selectioncolor_g = 0x6cd8,
    LineEdit_selectioncolor_b = 0x6cf2,
    LineEdit_selectioncolor_a = 0x6c2c,
    LineEdit_selectiontextcolor = 0x6c9b,
    LineEdit_selectiontextcolor_r = 0x6c46,
    LineEdit_selectiontextcolor_g = 0x6c81,
    LineEdit_selectiontextcolor_b = 0x6c5,
    LineEdit_selectiontextcolor_a = 0x6c7b,
    LineEdit_focused = 0x6c64,
    LineEdit_filter = 0x6c2d,

    SlideEdit = 0x5e00,
    SlideEdit_value = 0x5ea8,
    SlideEdit_precision = 0x5e17,
    SlideEdit_format = 0x5eb7,
    SlideEdit_minvalue = 0x5e2,
    SlideEdit_maxvalue = 0x5eaf,
    SlideEdit_handleimage = 0x5ee2,
    SlideEdit_handleimagetint = 0x5e11,
    SlideEdit_handleimagetint_r = 0x5e43,
    SlideEdit_handleimagetint_g = 0x5ef9,
    SlideEdit_handleimagetint_b = 0x5ea6,
    SlideEdit_handleimagetint_a = 0x5e29,
    SlideEdit_handleimagelayer = 0x5eec,
    SlideEdit_sliderate = 0x5e70,

    ListBox = 0x1100,
    ListBox_text = 0x1136,
    ListBox_textcolor = 0x1173,
    ListBox_textcolor_r = 0x1154,
    ListBox_textcolor_g = 0x11df,
    ListBox_textcolor_b = 0x115c,
    ListBox_textcolor_a = 0x115b,
    ListBox_handleimage = 0x11d2,
    ListBox_handletint = 0x1137,
    ListBox_handletint_r = 0x11db,
    ListBox_handletint_g = 0x1165,
    ListBox_handletint_b = 0x1161,
    ListBox_handletint_a = 0x11e2,
    ListBox_handlelayer = 0x1119,
    ListBox_handleslice = 0x11e0,
    ListBox_handlesize = 0x11d3,
    ListBox_handleinset = 0x117a,
    ListBox_padding = 0x11ca,
    ListBox_selectioncolor = 0x111d,
    ListBox_selectioncolor_r = 0x1199,
    ListBox_selectioncolor_g = 0x11aa,
    ListBox_selectioncolor_b = 0x118d,
    ListBox_selectioncolor_a = 0x11b2,
    ListBox_selectiontextcolor = 0x1176,
    ListBox_selectiontextcolor_r = 0x1170,
    ListBox_selectiontextcolor_g = 0x11c5,
    ListBox_selectiontextcolor_b = 0x11f7,
    ListBox_selectiontextcolor_a = 0x1115,
    ListBox_focused = 0x1116,

    ComboBox = 0xaf00,
    ComboBox_text = 0xafa6,
    ComboBox_textcolor = 0xaf8e,
    ComboBox_textcolor_r = 0xafe8,
    ComboBox_textcolor_g = 0xaf18,
    ComboBox_textcolor_b = 0xaf6a,
    ComboBox_textcolor_a = 0xafc8,
    ComboBox_dropimage = 0xafe3,
    ComboBox_dropimagetint = 0xaf44,
    ComboBox_dropimagetint_r = 0xafff,
    ComboBox_dropimagetint_g = 0xafe6,
    ComboBox_dropimagetint_b = 0xaff9,
    ComboBox_dropimagetint_a = 0xaffa,
    ComboBox_dropimagelayer = 0xafd5,
    ComboBox_listborderwidth = 0xaf94,
    ComboBox_listbordercolor = 0xafae,
    ComboBox_listbordercolor_r = 0xafdd,
    ComboBox_listbordercolor_g = 0xafe1,
    ComboBox_listbordercolor_b = 0xaf24,
    ComboBox_listbordercolor_a = 0xaf20,
    ComboBox_listbackgroundcolor = 0xaf56,
    ComboBox_listbackgroundcolor_r = 0xafc9,
    ComboBox_listbackgroundcolor_g = 0xaf31,
    ComboBox_listbackgroundcolor_b = 0xaf41,
    ComboBox_listbackgroundcolor_a = 0xaf42,
    ComboBox_handleimage = 0xaf32,
    ComboBox_handletint = 0xaf49,
    ComboBox_handletint_r = 0xaf33,
    ComboBox_handletint_g = 0xaf7b,
    ComboBox_handletint_b = 0xafb2,
    ComboBox_handletint_a = 0xafe,
    ComboBox_handlelayer = 0xafa1,
    ComboBox_handleslice = 0xafc5,
    ComboBox_handlesize = 0xaf92,
    ComboBox_handleinset = 0xaf1e,
    ComboBox_padding = 0xafa5,
    ComboBox_selectioncolor = 0xafbf,
    ComboBox_selectioncolor_r = 0xafc4,
    ComboBox_selectioncolor_g = 0xaf93,
    ComboBox_selectioncolor_b = 0xafb7,
    ComboBox_selectioncolor_a = 0xaf99,
    ComboBox_selectiontextcolor = 0xafb5,
    ComboBox_selectiontextcolor_r = 0xafd6,
    ComboBox_selectiontextcolor_g = 0xafce,
    ComboBox_selectiontextcolor_b = 0xaf8a,
    ComboBox_selectiontextcolor_a = 0xaf77,
    ComboBox_pressed = 0xaf35,
    ComboBox_dropped = 0xaf19,

    GroupBox = 0x2f00,

  };

  enum class Type : uint8_t
  {
    Int,
    Bool,
    Float,
    Color,
    Font,
    Sprite,
    Text,
    Item,
  };

  enum class Op : uint8_t
  {
    CreateItem = 0x9f,
    SetId = 0x6a,
    StoreInt = 0x2e,
    StoreBool = 0xa4,
    StoreFloat = 0xb6,
    StoreColor = 0x9d,
    StoreText = 0xb1,
    BeginExpr = 0x94,
    PatchExpr = 0xb8,
    StoreExpr = 0x64,
    StoreAnim = 0x35,
    Pop = 0xc7,
  };

  namespace
  {
    ///////////////////////// type_of //////////////////////////////////////////
    [[gnu::unused]] Type type_of(Id id)
    {
      switch(id)
      {
        case Id::Rectangle_borderwidth:
        case Id::Text_horizontalalign:
        case Id::Text_verticalalign:
        case Id::Image_fillmode:
        case Id::Image_horizontalalign:
        case Id::Image_verticalalign:
        case Id::ScrollBar_orientation:
        case Id::TextArea_horizontalalign:
        case Id::TextArea_verticalalign:
        case Id::Control_borderwidth:
        case Id::Control_action:
        case Id::Label_horizontalalign:
        case Id::Label_verticalalign:
        case Id::Field_precision:
        case Id::Field_horizontalalign:
        case Id::Field_verticalalign:
        case Id::Button_textalign:
        case Id::Button_imagealign:
        case Id::CheckButton_checked:
        case Id::Slider_orientation:
        case Id::LineEdit_filter:
        case Id::SlideEdit_precision:
        case Id::ComboBox_listborderwidth:
          return Type::Int;

        case Id::Item_clip:
        case Id::Item_visible:
        case Id::ScrollBar_hovered:
        case Id::ScrollBar_pressed:
        case Id::ScrollArea_wheelscroll:
        case Id::ScrollArea_interactive:
        case Id::TextArea_interactive:
        case Id::Control_enabled:
        case Id::Control_hovered:
        case Id::Button_pressed:
        case Id::Slider_pressed:
        case Id::LineEdit_focused:
        case Id::ListBox_focused:
        case Id::ComboBox_pressed:
        case Id::ComboBox_dropped:
          return Type::Bool;

        case Id::Item_x:
        case Id::Item_y:
        case Id::Item_width:
        case Id::Item_height:
        case Id::Item_z:
        case Id::Item_scale:
        case Id::Item_rotation:
        case Id::Item_opacity:
        case Id::Rectangle_color_r:
        case Id::Rectangle_color_g:
        case Id::Rectangle_color_b:
        case Id::Rectangle_color_a:
        case Id::Rectangle_bordercolor_r:
        case Id::Rectangle_bordercolor_g:
        case Id::Rectangle_bordercolor_b:
        case Id::Rectangle_bordercolor_a:
        case Id::Text_color_r:
        case Id::Text_color_g:
        case Id::Text_color_b:
        case Id::Text_color_a:
        case Id::Image_tint_r:
        case Id::Image_tint_g:
        case Id::Image_tint_b:
        case Id::Image_tint_a:
        case Id::Image_layer:
        case Id::Frame_tint_r:
        case Id::Frame_tint_g:
        case Id::Frame_tint_b:
        case Id::Frame_tint_a:
        case Id::Frame_layer:
        case Id::Frame_slice:
        case Id::Row_spacing:
        case Id::Column_spacing:
        case Id::ScrollBar_value:
        case Id::ScrollBar_size:
        case Id::ScrollBar_handletint_r:
        case Id::ScrollBar_handletint_g:
        case Id::ScrollBar_handletint_b:
        case Id::ScrollBar_handletint_a:
        case Id::ScrollBar_handlelayer:
        case Id::ScrollBar_handleslice:
        case Id::ScrollBar_handleinset:
        case Id::ScrollArea_contentwidth:
        case Id::ScrollArea_contentheight:
        case Id::ScrollArea_handletint_r:
        case Id::ScrollArea_handletint_g:
        case Id::ScrollArea_handletint_b:
        case Id::ScrollArea_handletint_a:
        case Id::ScrollArea_handlelayer:
        case Id::ScrollArea_handleslice:
        case Id::ScrollArea_handlesize:
        case Id::ScrollArea_handleinset:
        case Id::TextArea_textcolor_r:
        case Id::TextArea_textcolor_g:
        case Id::TextArea_textcolor_b:
        case Id::TextArea_textcolor_a:
        case Id::TextArea_selectioncolor_r:
        case Id::TextArea_selectioncolor_g:
        case Id::TextArea_selectioncolor_b:
        case Id::TextArea_selectioncolor_a:
        case Id::TextArea_selectiontextcolor_r:
        case Id::TextArea_selectiontextcolor_g:
        case Id::TextArea_selectiontextcolor_b:
        case Id::TextArea_selectiontextcolor_a:
        case Id::Control_bordercolor_r:
        case Id::Control_bordercolor_g:
        case Id::Control_bordercolor_b:
        case Id::Control_bordercolor_a:
        case Id::Control_backgroundcolor_r:
        case Id::Control_backgroundcolor_g:
        case Id::Control_backgroundcolor_b:
        case Id::Control_backgroundcolor_a:
        case Id::Control_backgroundlayer:
        case Id::Control_backgroundslice:
        case Id::Control_fontheight:
        case Id::Label_textcolor_r:
        case Id::Label_textcolor_g:
        case Id::Label_textcolor_b:
        case Id::Label_textcolor_a:
        case Id::Label_padding:
        case Id::Field_value:
        case Id::Field_textcolor_r:
        case Id::Field_textcolor_g:
        case Id::Field_textcolor_b:
        case Id::Field_textcolor_a:
        case Id::Field_padding:
        case Id::Button_textcolor_r:
        case Id::Button_textcolor_g:
        case Id::Button_textcolor_b:
        case Id::Button_textcolor_a:
        case Id::Button_imagetint_r:
        case Id::Button_imagetint_g:
        case Id::Button_imagetint_b:
        case Id::Button_imagetint_a:
        case Id::Button_imagelayer:
        case Id::Button_spacing:
        case Id::Button_padding:
        case Id::CheckButton_innercolor_r:
        case Id::CheckButton_innercolor_g:
        case Id::CheckButton_innercolor_b:
        case Id::CheckButton_innercolor_a:
        case Id::CheckButton_outercolor_r:
        case Id::CheckButton_outercolor_g:
        case Id::CheckButton_outercolor_b:
        case Id::CheckButton_outercolor_a:
        case Id::DragButton_dx:
        case Id::DragButton_dy:
        case Id::Slider_value:
        case Id::Slider_minvalue:
        case Id::Slider_maxvalue:
        case Id::Slider_inset:
        case Id::Slider_handletint_r:
        case Id::Slider_handletint_g:
        case Id::Slider_handletint_b:
        case Id::Slider_handletint_a:
        case Id::Slider_handlelayer:
        case Id::LineEdit_textcolor_r:
        case Id::LineEdit_textcolor_g:
        case Id::LineEdit_textcolor_b:
        case Id::LineEdit_textcolor_a:
        case Id::LineEdit_padding:
        case Id::LineEdit_selectioncolor_r:
        case Id::LineEdit_selectioncolor_g:
        case Id::LineEdit_selectioncolor_b:
        case Id::LineEdit_selectioncolor_a:
        case Id::LineEdit_selectiontextcolor_r:
        case Id::LineEdit_selectiontextcolor_g:
        case Id::LineEdit_selectiontextcolor_b:
        case Id::LineEdit_selectiontextcolor_a:
        case Id::SlideEdit_value:
        case Id::SlideEdit_minvalue:
        case Id::SlideEdit_maxvalue:
        case Id::SlideEdit_handleimagetint_r:
        case Id::SlideEdit_handleimagetint_g:
        case Id::SlideEdit_handleimagetint_b:
        case Id::SlideEdit_handleimagetint_a:
        case Id::SlideEdit_handleimagelayer:
        case Id::SlideEdit_sliderate:
        case Id::ListBox_textcolor_r:
        case Id::ListBox_textcolor_g:
        case Id::ListBox_textcolor_b:
        case Id::ListBox_textcolor_a:
        case Id::ListBox_handletint_r:
        case Id::ListBox_handletint_g:
        case Id::ListBox_handletint_b:
        case Id::ListBox_handletint_a:
        case Id::ListBox_handlelayer:
        case Id::ListBox_handleslice:
        case Id::ListBox_handlesize:
        case Id::ListBox_handleinset:
        case Id::ListBox_padding:
        case Id::ListBox_selectioncolor_r:
        case Id::ListBox_selectioncolor_g:
        case Id::ListBox_selectioncolor_b:
        case Id::ListBox_selectioncolor_a:
        case Id::ListBox_selectiontextcolor_r:
        case Id::ListBox_selectiontextcolor_g:
        case Id::ListBox_selectiontextcolor_b:
        case Id::ListBox_selectiontextcolor_a:
        case Id::ComboBox_textcolor_r:
        case Id::ComboBox_textcolor_g:
        case Id::ComboBox_textcolor_b:
        case Id::ComboBox_textcolor_a:
        case Id::ComboBox_dropimagetint_r:
        case Id::ComboBox_dropimagetint_g:
        case Id::ComboBox_dropimagetint_b:
        case Id::ComboBox_dropimagetint_a:
        case Id::ComboBox_dropimagelayer:
        case Id::ComboBox_listbordercolor_r:
        case Id::ComboBox_listbordercolor_g:
        case Id::ComboBox_listbordercolor_b:
        case Id::ComboBox_listbordercolor_a:
        case Id::ComboBox_listbackgroundcolor_r:
        case Id::ComboBox_listbackgroundcolor_g:
        case Id::ComboBox_listbackgroundcolor_b:
        case Id::ComboBox_listbackgroundcolor_a:
        case Id::ComboBox_handletint_r:
        case Id::ComboBox_handletint_g:
        case Id::ComboBox_handletint_b:
        case Id::ComboBox_handletint_a:
        case Id::ComboBox_handlelayer:
        case Id::ComboBox_handleslice:
        case Id::ComboBox_handlesize:
        case Id::ComboBox_handleinset:
        case Id::ComboBox_padding:
        case Id::ComboBox_selectioncolor_r:
        case Id::ComboBox_selectioncolor_g:
        case Id::ComboBox_selectioncolor_b:
        case Id::ComboBox_selectioncolor_a:
        case Id::ComboBox_selectiontextcolor_r:
        case Id::ComboBox_selectiontextcolor_g:
        case Id::ComboBox_selectiontextcolor_b:
        case Id::ComboBox_selectiontextcolor_a:
          return Type::Float;

        case Id::Rectangle_color:
        case Id::Rectangle_bordercolor:
        case Id::Text_color:
        case Id::Image_tint:
        case Id::Frame_tint:
        case Id::ScrollBar_handletint:
        case Id::ScrollArea_handletint:
        case Id::TextArea_textcolor:
        case Id::TextArea_selectioncolor:
        case Id::TextArea_selectiontextcolor:
        case Id::Control_bordercolor:
        case Id::Control_backgroundcolor:
        case Id::Label_textcolor:
        case Id::Field_textcolor:
        case Id::Button_textcolor:
        case Id::Button_imagetint:
        case Id::CheckButton_innercolor:
        case Id::CheckButton_outercolor:
        case Id::Slider_handletint:
        case Id::LineEdit_textcolor:
        case Id::LineEdit_selectioncolor:
        case Id::LineEdit_selectiontextcolor:
        case Id::SlideEdit_handleimagetint:
        case Id::ListBox_textcolor:
        case Id::ListBox_handletint:
        case Id::ListBox_selectioncolor:
        case Id::ListBox_selectiontextcolor:
        case Id::ComboBox_textcolor:
        case Id::ComboBox_dropimagetint:
        case Id::ComboBox_listbordercolor:
        case Id::ComboBox_listbackgroundcolor:
        case Id::ComboBox_handletint:
        case Id::ComboBox_selectioncolor:
        case Id::ComboBox_selectiontextcolor:
          return Type::Color;

        case Id::Text_font:
        case Id::TextArea_font:
        case Id::Control_font:
          return Type::Font;

        case Id::Image_image:
        case Id::Frame_image:
        case Id::ScrollBar_handleimage:
        case Id::ScrollArea_handleimage:
        case Id::Control_backgroundimage:
        case Id::Button_image:
        case Id::Slider_handleimage:
        case Id::SlideEdit_handleimage:
        case Id::ListBox_handleimage:
        case Id::ComboBox_dropimage:
        case Id::ComboBox_handleimage:
          return Type::Sprite;

        case Id::Text_text:
        case Id::TextArea_text:
        case Id::Label_text:
        case Id::Field_format:
        case Id::Button_text:
        case Id::LineEdit_text:
        case Id::SlideEdit_format:
        case Id::ListBox_text:
        case Id::ComboBox_text:
          return Type::Text;

        case Id::Item:
        case Id::Rectangle:
        case Id::Text:
        case Id::Image:
        case Id::Frame:
        case Id::Row:
        case Id::Column:
        case Id::ScrollBar:
        case Id::ScrollArea:
        case Id::TextArea:
        case Id::Control:
        case Id::Label:
        case Id::Field:
        case Id::Button:
        case Id::CheckButton:
        case Id::DragButton:
        case Id::Slider:
        case Id::LineEdit:
        case Id::SlideEdit:
        case Id::ListBox:
        case Id::ComboBox:
        case Id::GroupBox:
          return Type::Item;

      }

      throw std::runtime_error("Unknown Type");
    }
    ///////////////////////// address_of //////////////////////////////////////////

    [[gnu::unused]] int *address_of_int(Item *item, Id id)
    {
      switch(id)
      {
        case Id::Rectangle_borderwidth:
          return &static_cast<Rectangle*>(item)->borderwidth;

        case Id::Text_horizontalalign:
          return &static_cast<Text*>(item)->horizontalalign;

        case Id::Text_verticalalign:
          return &static_cast<Text*>(item)->verticalalign;

        case Id::Image_fillmode:
          return &static_cast<Image*>(item)->fillmode;

        case Id::Image_horizontalalign:
          return &static_cast<Image*>(item)->horizontalalign;

        case Id::Image_verticalalign:
          return &static_cast<Image*>(item)->verticalalign;

        case Id::ScrollBar_orientation:
          return &static_cast<ScrollBar*>(item)->orientation;

        case Id::TextArea_horizontalalign:
          return &static_cast<TextArea*>(item)->horizontalalign;

        case Id::TextArea_verticalalign:
          return &static_cast<TextArea*>(item)->verticalalign;

        case Id::Control_borderwidth:
          return &static_cast<Control*>(item)->borderwidth;

        case Id::Control_action:
          return &static_cast<Control*>(item)->action;

        case Id::Label_horizontalalign:
          return &static_cast<Label*>(item)->horizontalalign;

        case Id::Label_verticalalign:
          return &static_cast<Label*>(item)->verticalalign;

        case Id::Field_precision:
          return &static_cast<Field*>(item)->precision;

        case Id::Field_horizontalalign:
          return &static_cast<Field*>(item)->horizontalalign;

        case Id::Field_verticalalign:
          return &static_cast<Field*>(item)->verticalalign;

        case Id::Button_textalign:
          return &static_cast<Button*>(item)->textalign;

        case Id::Button_imagealign:
          return &static_cast<Button*>(item)->imagealign;

        case Id::CheckButton_checked:
          return &static_cast<CheckButton*>(item)->checked;

        case Id::Slider_orientation:
          return &static_cast<Slider*>(item)->orientation;

        case Id::LineEdit_filter:
          return &static_cast<LineEdit*>(item)->filter;

        case Id::SlideEdit_precision:
          return &static_cast<SlideEdit*>(item)->precision;

        case Id::ComboBox_listborderwidth:
          return &static_cast<ComboBox*>(item)->listborderwidth;

        default:
          throw std::runtime_error("Invalid Int Id");
      }
    }

    [[gnu::unused]] bool *address_of_bool(Item *item, Id id)
    {
      switch(id)
      {
        case Id::Item_clip:
          return &static_cast<Item*>(item)->clip;

        case Id::Item_visible:
          return &static_cast<Item*>(item)->visible;

        case Id::ScrollBar_hovered:
          return &static_cast<ScrollBar*>(item)->hovered;

        case Id::ScrollBar_pressed:
          return &static_cast<ScrollBar*>(item)->pressed;

        case Id::ScrollArea_wheelscroll:
          return &static_cast<ScrollArea*>(item)->wheelscroll;

        case Id::ScrollArea_interactive:
          return &static_cast<ScrollArea*>(item)->interactive;

        case Id::TextArea_interactive:
          return &static_cast<TextArea*>(item)->interactive;

        case Id::Control_enabled:
          return &static_cast<Control*>(item)->enabled;

        case Id::Control_hovered:
          return &static_cast<Control*>(item)->hovered;

        case Id::Button_pressed:
          return &static_cast<Button*>(item)->pressed;

        case Id::Slider_pressed:
          return &static_cast<Slider*>(item)->pressed;

        case Id::LineEdit_focused:
          return &static_cast<LineEdit*>(item)->focused;

        case Id::ListBox_focused:
          return &static_cast<ListBox*>(item)->focused;

        case Id::ComboBox_pressed:
          return &static_cast<ComboBox*>(item)->pressed;

        case Id::ComboBox_dropped:
          return &static_cast<ComboBox*>(item)->dropped;

        default:
          throw std::runtime_error("Invalid Bool Id");
      }
    }

    [[gnu::unused]] float *address_of_float(Item *item, Id id)
    {
      switch(id)
      {
        case Id::Item_x:
          return &static_cast<Item*>(item)->x;

        case Id::Item_y:
          return &static_cast<Item*>(item)->y;

        case Id::Item_width:
          return &static_cast<Item*>(item)->width;

        case Id::Item_height:
          return &static_cast<Item*>(item)->height;

        case Id::Item_z:
          return &static_cast<Item*>(item)->z;

        case Id::Item_scale:
          return &static_cast<Item*>(item)->scale;

        case Id::Item_rotation:
          return &static_cast<Item*>(item)->rotation;

        case Id::Item_opacity:
          return &static_cast<Item*>(item)->opacity;

        case Id::Rectangle_color_r:
          return &static_cast<Rectangle*>(item)->color.r;

        case Id::Rectangle_color_g:
          return &static_cast<Rectangle*>(item)->color.g;

        case Id::Rectangle_color_b:
          return &static_cast<Rectangle*>(item)->color.b;

        case Id::Rectangle_color_a:
          return &static_cast<Rectangle*>(item)->color.a;

        case Id::Rectangle_bordercolor_r:
          return &static_cast<Rectangle*>(item)->bordercolor.r;

        case Id::Rectangle_bordercolor_g:
          return &static_cast<Rectangle*>(item)->bordercolor.g;

        case Id::Rectangle_bordercolor_b:
          return &static_cast<Rectangle*>(item)->bordercolor.b;

        case Id::Rectangle_bordercolor_a:
          return &static_cast<Rectangle*>(item)->bordercolor.a;

        case Id::Text_color_r:
          return &static_cast<Text*>(item)->color.r;

        case Id::Text_color_g:
          return &static_cast<Text*>(item)->color.g;

        case Id::Text_color_b:
          return &static_cast<Text*>(item)->color.b;

        case Id::Text_color_a:
          return &static_cast<Text*>(item)->color.a;

        case Id::Image_tint_r:
          return &static_cast<Image*>(item)->tint.r;

        case Id::Image_tint_g:
          return &static_cast<Image*>(item)->tint.g;

        case Id::Image_tint_b:
          return &static_cast<Image*>(item)->tint.b;

        case Id::Image_tint_a:
          return &static_cast<Image*>(item)->tint.a;

        case Id::Image_layer:
          return &static_cast<Image*>(item)->layer;

        case Id::Frame_tint_r:
          return &static_cast<Frame*>(item)->tint.r;

        case Id::Frame_tint_g:
          return &static_cast<Frame*>(item)->tint.g;

        case Id::Frame_tint_b:
          return &static_cast<Frame*>(item)->tint.b;

        case Id::Frame_tint_a:
          return &static_cast<Frame*>(item)->tint.a;

        case Id::Frame_layer:
          return &static_cast<Frame*>(item)->layer;

        case Id::Frame_slice:
          return &static_cast<Frame*>(item)->slice;

        case Id::Row_spacing:
          return &static_cast<Row*>(item)->spacing;

        case Id::Column_spacing:
          return &static_cast<Column*>(item)->spacing;

        case Id::ScrollBar_value:
          return &static_cast<ScrollBar*>(item)->value;

        case Id::ScrollBar_size:
          return &static_cast<ScrollBar*>(item)->size;

        case Id::ScrollBar_handletint_r:
          return &static_cast<ScrollBar*>(item)->handletint.r;

        case Id::ScrollBar_handletint_g:
          return &static_cast<ScrollBar*>(item)->handletint.g;

        case Id::ScrollBar_handletint_b:
          return &static_cast<ScrollBar*>(item)->handletint.b;

        case Id::ScrollBar_handletint_a:
          return &static_cast<ScrollBar*>(item)->handletint.a;

        case Id::ScrollBar_handlelayer:
          return &static_cast<ScrollBar*>(item)->handlelayer;

        case Id::ScrollBar_handleslice:
          return &static_cast<ScrollBar*>(item)->handleslice;

        case Id::ScrollBar_handleinset:
          return &static_cast<ScrollBar*>(item)->handleinset;

        case Id::ScrollArea_contentwidth:
          return &static_cast<ScrollArea*>(item)->contentwidth;

        case Id::ScrollArea_contentheight:
          return &static_cast<ScrollArea*>(item)->contentheight;

        case Id::ScrollArea_handletint_r:
          return &static_cast<ScrollArea*>(item)->handletint.r;

        case Id::ScrollArea_handletint_g:
          return &static_cast<ScrollArea*>(item)->handletint.g;

        case Id::ScrollArea_handletint_b:
          return &static_cast<ScrollArea*>(item)->handletint.b;

        case Id::ScrollArea_handletint_a:
          return &static_cast<ScrollArea*>(item)->handletint.a;

        case Id::ScrollArea_handlelayer:
          return &static_cast<ScrollArea*>(item)->handlelayer;

        case Id::ScrollArea_handleslice:
          return &static_cast<ScrollArea*>(item)->handleslice;

        case Id::ScrollArea_handlesize:
          return &static_cast<ScrollArea*>(item)->handlesize;

        case Id::ScrollArea_handleinset:
          return &static_cast<ScrollArea*>(item)->handleinset;

        case Id::TextArea_textcolor_r:
          return &static_cast<TextArea*>(item)->textcolor.r;

        case Id::TextArea_textcolor_g:
          return &static_cast<TextArea*>(item)->textcolor.g;

        case Id::TextArea_textcolor_b:
          return &static_cast<TextArea*>(item)->textcolor.b;

        case Id::TextArea_textcolor_a:
          return &static_cast<TextArea*>(item)->textcolor.a;

        case Id::TextArea_selectioncolor_r:
          return &static_cast<TextArea*>(item)->selectioncolor.r;

        case Id::TextArea_selectioncolor_g:
          return &static_cast<TextArea*>(item)->selectioncolor.g;

        case Id::TextArea_selectioncolor_b:
          return &static_cast<TextArea*>(item)->selectioncolor.b;

        case Id::TextArea_selectioncolor_a:
          return &static_cast<TextArea*>(item)->selectioncolor.a;

        case Id::TextArea_selectiontextcolor_r:
          return &static_cast<TextArea*>(item)->selectiontextcolor.r;

        case Id::TextArea_selectiontextcolor_g:
          return &static_cast<TextArea*>(item)->selectiontextcolor.g;

        case Id::TextArea_selectiontextcolor_b:
          return &static_cast<TextArea*>(item)->selectiontextcolor.b;

        case Id::TextArea_selectiontextcolor_a:
          return &static_cast<TextArea*>(item)->selectiontextcolor.a;

        case Id::Control_bordercolor_r:
          return &static_cast<Control*>(item)->bordercolor.r;

        case Id::Control_bordercolor_g:
          return &static_cast<Control*>(item)->bordercolor.g;

        case Id::Control_bordercolor_b:
          return &static_cast<Control*>(item)->bordercolor.b;

        case Id::Control_bordercolor_a:
          return &static_cast<Control*>(item)->bordercolor.a;

        case Id::Control_backgroundcolor_r:
          return &static_cast<Control*>(item)->backgroundcolor.r;

        case Id::Control_backgroundcolor_g:
          return &static_cast<Control*>(item)->backgroundcolor.g;

        case Id::Control_backgroundcolor_b:
          return &static_cast<Control*>(item)->backgroundcolor.b;

        case Id::Control_backgroundcolor_a:
          return &static_cast<Control*>(item)->backgroundcolor.a;

        case Id::Control_backgroundlayer:
          return &static_cast<Control*>(item)->backgroundlayer;

        case Id::Control_backgroundslice:
          return &static_cast<Control*>(item)->backgroundslice;

        case Id::Control_fontheight:
          return &static_cast<Control*>(item)->fontheight;

        case Id::Label_textcolor_r:
          return &static_cast<Label*>(item)->textcolor.r;

        case Id::Label_textcolor_g:
          return &static_cast<Label*>(item)->textcolor.g;

        case Id::Label_textcolor_b:
          return &static_cast<Label*>(item)->textcolor.b;

        case Id::Label_textcolor_a:
          return &static_cast<Label*>(item)->textcolor.a;

        case Id::Label_padding:
          return &static_cast<Label*>(item)->padding;

        case Id::Field_value:
          return &static_cast<Field*>(item)->value;

        case Id::Field_textcolor_r:
          return &static_cast<Field*>(item)->textcolor.r;

        case Id::Field_textcolor_g:
          return &static_cast<Field*>(item)->textcolor.g;

        case Id::Field_textcolor_b:
          return &static_cast<Field*>(item)->textcolor.b;

        case Id::Field_textcolor_a:
          return &static_cast<Field*>(item)->textcolor.a;

        case Id::Field_padding:
          return &static_cast<Field*>(item)->padding;

        case Id::Button_textcolor_r:
          return &static_cast<Button*>(item)->textcolor.r;

        case Id::Button_textcolor_g:
          return &static_cast<Button*>(item)->textcolor.g;

        case Id::Button_textcolor_b:
          return &static_cast<Button*>(item)->textcolor.b;

        case Id::Button_textcolor_a:
          return &static_cast<Button*>(item)->textcolor.a;

        case Id::Button_imagetint_r:
          return &static_cast<Button*>(item)->imagetint.r;

        case Id::Button_imagetint_g:
          return &static_cast<Button*>(item)->imagetint.g;

        case Id::Button_imagetint_b:
          return &static_cast<Button*>(item)->imagetint.b;

        case Id::Button_imagetint_a:
          return &static_cast<Button*>(item)->imagetint.a;

        case Id::Button_imagelayer:
          return &static_cast<Button*>(item)->imagelayer;

        case Id::Button_spacing:
          return &static_cast<Button*>(item)->spacing;

        case Id::Button_padding:
          return &static_cast<Button*>(item)->padding;

        case Id::CheckButton_innercolor_r:
          return &static_cast<CheckButton*>(item)->innercolor.r;

        case Id::CheckButton_innercolor_g:
          return &static_cast<CheckButton*>(item)->innercolor.g;

        case Id::CheckButton_innercolor_b:
          return &static_cast<CheckButton*>(item)->innercolor.b;

        case Id::CheckButton_innercolor_a:
          return &static_cast<CheckButton*>(item)->innercolor.a;

        case Id::CheckButton_outercolor_r:
          return &static_cast<CheckButton*>(item)->outercolor.r;

        case Id::CheckButton_outercolor_g:
          return &static_cast<CheckButton*>(item)->outercolor.g;

        case Id::CheckButton_outercolor_b:
          return &static_cast<CheckButton*>(item)->outercolor.b;

        case Id::CheckButton_outercolor_a:
          return &static_cast<CheckButton*>(item)->outercolor.a;

        case Id::DragButton_dx:
          return &static_cast<DragButton*>(item)->dx;

        case Id::DragButton_dy:
          return &static_cast<DragButton*>(item)->dy;

        case Id::Slider_value:
          return &static_cast<Slider*>(item)->value;

        case Id::Slider_minvalue:
          return &static_cast<Slider*>(item)->minvalue;

        case Id::Slider_maxvalue:
          return &static_cast<Slider*>(item)->maxvalue;

        case Id::Slider_inset:
          return &static_cast<Slider*>(item)->inset;

        case Id::Slider_handletint_r:
          return &static_cast<Slider*>(item)->handletint.r;

        case Id::Slider_handletint_g:
          return &static_cast<Slider*>(item)->handletint.g;

        case Id::Slider_handletint_b:
          return &static_cast<Slider*>(item)->handletint.b;

        case Id::Slider_handletint_a:
          return &static_cast<Slider*>(item)->handletint.a;

        case Id::Slider_handlelayer:
          return &static_cast<Slider*>(item)->handlelayer;

        case Id::LineEdit_textcolor_r:
          return &static_cast<LineEdit*>(item)->textcolor.r;

        case Id::LineEdit_textcolor_g:
          return &static_cast<LineEdit*>(item)->textcolor.g;

        case Id::LineEdit_textcolor_b:
          return &static_cast<LineEdit*>(item)->textcolor.b;

        case Id::LineEdit_textcolor_a:
          return &static_cast<LineEdit*>(item)->textcolor.a;

        case Id::LineEdit_padding:
          return &static_cast<LineEdit*>(item)->padding;

        case Id::LineEdit_selectioncolor_r:
          return &static_cast<LineEdit*>(item)->selectioncolor.r;

        case Id::LineEdit_selectioncolor_g:
          return &static_cast<LineEdit*>(item)->selectioncolor.g;

        case Id::LineEdit_selectioncolor_b:
          return &static_cast<LineEdit*>(item)->selectioncolor.b;

        case Id::LineEdit_selectioncolor_a:
          return &static_cast<LineEdit*>(item)->selectioncolor.a;

        case Id::LineEdit_selectiontextcolor_r:
          return &static_cast<LineEdit*>(item)->selectiontextcolor.r;

        case Id::LineEdit_selectiontextcolor_g:
          return &static_cast<LineEdit*>(item)->selectiontextcolor.g;

        case Id::LineEdit_selectiontextcolor_b:
          return &static_cast<LineEdit*>(item)->selectiontextcolor.b;

        case Id::LineEdit_selectiontextcolor_a:
          return &static_cast<LineEdit*>(item)->selectiontextcolor.a;

        case Id::SlideEdit_value:
          return &static_cast<SlideEdit*>(item)->value;

        case Id::SlideEdit_minvalue:
          return &static_cast<SlideEdit*>(item)->minvalue;

        case Id::SlideEdit_maxvalue:
          return &static_cast<SlideEdit*>(item)->maxvalue;

        case Id::SlideEdit_handleimagetint_r:
          return &static_cast<SlideEdit*>(item)->handleimagetint.r;

        case Id::SlideEdit_handleimagetint_g:
          return &static_cast<SlideEdit*>(item)->handleimagetint.g;

        case Id::SlideEdit_handleimagetint_b:
          return &static_cast<SlideEdit*>(item)->handleimagetint.b;

        case Id::SlideEdit_handleimagetint_a:
          return &static_cast<SlideEdit*>(item)->handleimagetint.a;

        case Id::SlideEdit_handleimagelayer:
          return &static_cast<SlideEdit*>(item)->handleimagelayer;

        case Id::SlideEdit_sliderate:
          return &static_cast<SlideEdit*>(item)->sliderate;

        case Id::ListBox_textcolor_r:
          return &static_cast<ListBox*>(item)->textcolor.r;

        case Id::ListBox_textcolor_g:
          return &static_cast<ListBox*>(item)->textcolor.g;

        case Id::ListBox_textcolor_b:
          return &static_cast<ListBox*>(item)->textcolor.b;

        case Id::ListBox_textcolor_a:
          return &static_cast<ListBox*>(item)->textcolor.a;

        case Id::ListBox_handletint_r:
          return &static_cast<ListBox*>(item)->handletint.r;

        case Id::ListBox_handletint_g:
          return &static_cast<ListBox*>(item)->handletint.g;

        case Id::ListBox_handletint_b:
          return &static_cast<ListBox*>(item)->handletint.b;

        case Id::ListBox_handletint_a:
          return &static_cast<ListBox*>(item)->handletint.a;

        case Id::ListBox_handlelayer:
          return &static_cast<ListBox*>(item)->handlelayer;

        case Id::ListBox_handleslice:
          return &static_cast<ListBox*>(item)->handleslice;

        case Id::ListBox_handlesize:
          return &static_cast<ListBox*>(item)->handlesize;

        case Id::ListBox_handleinset:
          return &static_cast<ListBox*>(item)->handleinset;

        case Id::ListBox_padding:
          return &static_cast<ListBox*>(item)->padding;

        case Id::ListBox_selectioncolor_r:
          return &static_cast<ListBox*>(item)->selectioncolor.r;

        case Id::ListBox_selectioncolor_g:
          return &static_cast<ListBox*>(item)->selectioncolor.g;

        case Id::ListBox_selectioncolor_b:
          return &static_cast<ListBox*>(item)->selectioncolor.b;

        case Id::ListBox_selectioncolor_a:
          return &static_cast<ListBox*>(item)->selectioncolor.a;

        case Id::ListBox_selectiontextcolor_r:
          return &static_cast<ListBox*>(item)->selectiontextcolor.r;

        case Id::ListBox_selectiontextcolor_g:
          return &static_cast<ListBox*>(item)->selectiontextcolor.g;

        case Id::ListBox_selectiontextcolor_b:
          return &static_cast<ListBox*>(item)->selectiontextcolor.b;

        case Id::ListBox_selectiontextcolor_a:
          return &static_cast<ListBox*>(item)->selectiontextcolor.a;

        case Id::ComboBox_textcolor_r:
          return &static_cast<ComboBox*>(item)->textcolor.r;

        case Id::ComboBox_textcolor_g:
          return &static_cast<ComboBox*>(item)->textcolor.g;

        case Id::ComboBox_textcolor_b:
          return &static_cast<ComboBox*>(item)->textcolor.b;

        case Id::ComboBox_textcolor_a:
          return &static_cast<ComboBox*>(item)->textcolor.a;

        case Id::ComboBox_dropimagetint_r:
          return &static_cast<ComboBox*>(item)->dropimagetint.r;

        case Id::ComboBox_dropimagetint_g:
          return &static_cast<ComboBox*>(item)->dropimagetint.g;

        case Id::ComboBox_dropimagetint_b:
          return &static_cast<ComboBox*>(item)->dropimagetint.b;

        case Id::ComboBox_dropimagetint_a:
          return &static_cast<ComboBox*>(item)->dropimagetint.a;

        case Id::ComboBox_dropimagelayer:
          return &static_cast<ComboBox*>(item)->dropimagelayer;

        case Id::ComboBox_listbordercolor_r:
          return &static_cast<ComboBox*>(item)->listbordercolor.r;

        case Id::ComboBox_listbordercolor_g:
          return &static_cast<ComboBox*>(item)->listbordercolor.g;

        case Id::ComboBox_listbordercolor_b:
          return &static_cast<ComboBox*>(item)->listbordercolor.b;

        case Id::ComboBox_listbordercolor_a:
          return &static_cast<ComboBox*>(item)->listbordercolor.a;

        case Id::ComboBox_listbackgroundcolor_r:
          return &static_cast<ComboBox*>(item)->listbackgroundcolor.r;

        case Id::ComboBox_listbackgroundcolor_g:
          return &static_cast<ComboBox*>(item)->listbackgroundcolor.g;

        case Id::ComboBox_listbackgroundcolor_b:
          return &static_cast<ComboBox*>(item)->listbackgroundcolor.b;

        case Id::ComboBox_listbackgroundcolor_a:
          return &static_cast<ComboBox*>(item)->listbackgroundcolor.a;

        case Id::ComboBox_handletint_r:
          return &static_cast<ComboBox*>(item)->handletint.r;

        case Id::ComboBox_handletint_g:
          return &static_cast<ComboBox*>(item)->handletint.g;

        case Id::ComboBox_handletint_b:
          return &static_cast<ComboBox*>(item)->handletint.b;

        case Id::ComboBox_handletint_a:
          return &static_cast<ComboBox*>(item)->handletint.a;

        case Id::ComboBox_handlelayer:
          return &static_cast<ComboBox*>(item)->handlelayer;

        case Id::ComboBox_handleslice:
          return &static_cast<ComboBox*>(item)->handleslice;

        case Id::ComboBox_handlesize:
          return &static_cast<ComboBox*>(item)->handlesize;

        case Id::ComboBox_handleinset:
          return &static_cast<ComboBox*>(item)->handleinset;

        case Id::ComboBox_padding:
          return &static_cast<ComboBox*>(item)->padding;

        case Id::ComboBox_selectioncolor_r:
          return &static_cast<ComboBox*>(item)->selectioncolor.r;

        case Id::ComboBox_selectioncolor_g:
          return &static_cast<ComboBox*>(item)->selectioncolor.g;

        case Id::ComboBox_selectioncolor_b:
          return &static_cast<ComboBox*>(item)->selectioncolor.b;

        case Id::ComboBox_selectioncolor_a:
          return &static_cast<ComboBox*>(item)->selectioncolor.a;

        case Id::ComboBox_selectiontextcolor_r:
          return &static_cast<ComboBox*>(item)->selectiontextcolor.r;

        case Id::ComboBox_selectiontextcolor_g:
          return &static_cast<ComboBox*>(item)->selectiontextcolor.g;

        case Id::ComboBox_selectiontextcolor_b:
          return &static_cast<ComboBox*>(item)->selectiontextcolor.b;

        case Id::ComboBox_selectiontextcolor_a:
          return &static_cast<ComboBox*>(item)->selectiontextcolor.a;

        default:
          throw std::runtime_error("Invalid Float Id");
      }
    }

    [[gnu::unused]] Color4 *address_of_color(Item *item, Id id)
    {
      switch(id)
      {
        case Id::Rectangle_color:
          return &static_cast<Rectangle*>(item)->color;

        case Id::Rectangle_bordercolor:
          return &static_cast<Rectangle*>(item)->bordercolor;

        case Id::Text_color:
          return &static_cast<Text*>(item)->color;

        case Id::Image_tint:
          return &static_cast<Image*>(item)->tint;

        case Id::Frame_tint:
          return &static_cast<Frame*>(item)->tint;

        case Id::ScrollBar_handletint:
          return &static_cast<ScrollBar*>(item)->handletint;

        case Id::ScrollArea_handletint:
          return &static_cast<ScrollArea*>(item)->handletint;

        case Id::TextArea_textcolor:
          return &static_cast<TextArea*>(item)->textcolor;

        case Id::TextArea_selectioncolor:
          return &static_cast<TextArea*>(item)->selectioncolor;

        case Id::TextArea_selectiontextcolor:
          return &static_cast<TextArea*>(item)->selectiontextcolor;

        case Id::Control_bordercolor:
          return &static_cast<Control*>(item)->bordercolor;

        case Id::Control_backgroundcolor:
          return &static_cast<Control*>(item)->backgroundcolor;

        case Id::Label_textcolor:
          return &static_cast<Label*>(item)->textcolor;

        case Id::Field_textcolor:
          return &static_cast<Field*>(item)->textcolor;

        case Id::Button_textcolor:
          return &static_cast<Button*>(item)->textcolor;

        case Id::Button_imagetint:
          return &static_cast<Button*>(item)->imagetint;

        case Id::CheckButton_innercolor:
          return &static_cast<CheckButton*>(item)->innercolor;

        case Id::CheckButton_outercolor:
          return &static_cast<CheckButton*>(item)->outercolor;

        case Id::Slider_handletint:
          return &static_cast<Slider*>(item)->handletint;

        case Id::LineEdit_textcolor:
          return &static_cast<LineEdit*>(item)->textcolor;

        case Id::LineEdit_selectioncolor:
          return &static_cast<LineEdit*>(item)->selectioncolor;

        case Id::LineEdit_selectiontextcolor:
          return &static_cast<LineEdit*>(item)->selectiontextcolor;

        case Id::SlideEdit_handleimagetint:
          return &static_cast<SlideEdit*>(item)->handleimagetint;

        case Id::ListBox_textcolor:
          return &static_cast<ListBox*>(item)->textcolor;

        case Id::ListBox_handletint:
          return &static_cast<ListBox*>(item)->handletint;

        case Id::ListBox_selectioncolor:
          return &static_cast<ListBox*>(item)->selectioncolor;

        case Id::ListBox_selectiontextcolor:
          return &static_cast<ListBox*>(item)->selectiontextcolor;

        case Id::ComboBox_textcolor:
          return &static_cast<ComboBox*>(item)->textcolor;

        case Id::ComboBox_dropimagetint:
          return &static_cast<ComboBox*>(item)->dropimagetint;

        case Id::ComboBox_listbordercolor:
          return &static_cast<ComboBox*>(item)->listbordercolor;

        case Id::ComboBox_listbackgroundcolor:
          return &static_cast<ComboBox*>(item)->listbackgroundcolor;

        case Id::ComboBox_handletint:
          return &static_cast<ComboBox*>(item)->handletint;

        case Id::ComboBox_selectioncolor:
          return &static_cast<ComboBox*>(item)->selectioncolor;

        case Id::ComboBox_selectiontextcolor:
          return &static_cast<ComboBox*>(item)->selectiontextcolor;

        default:
          throw std::runtime_error("Invalid Color Id");
      }
    }

    [[gnu::unused]] Font *address_of_font(Item *item, Id id)
    {
      switch(id)
      {
        case Id::Text_font:
          return &static_cast<Text*>(item)->font;

        case Id::TextArea_font:
          return &static_cast<TextArea*>(item)->font;

        case Id::Control_font:
          return &static_cast<Control*>(item)->font;

        default:
          throw std::runtime_error("Invalid Font Id");
      }
    }

    [[gnu::unused]] Sprite *address_of_sprite(Item *item, Id id)
    {
      switch(id)
      {
        case Id::Image_image:
          return &static_cast<Image*>(item)->image;

        case Id::Frame_image:
          return &static_cast<Frame*>(item)->image;

        case Id::ScrollBar_handleimage:
          return &static_cast<ScrollBar*>(item)->handleimage;

        case Id::ScrollArea_handleimage:
          return &static_cast<ScrollArea*>(item)->handleimage;

        case Id::Control_backgroundimage:
          return &static_cast<Control*>(item)->backgroundimage;

        case Id::Button_image:
          return &static_cast<Button*>(item)->image;

        case Id::Slider_handleimage:
          return &static_cast<Slider*>(item)->handleimage;

        case Id::SlideEdit_handleimage:
          return &static_cast<SlideEdit*>(item)->handleimage;

        case Id::ListBox_handleimage:
          return &static_cast<ListBox*>(item)->handleimage;

        case Id::ComboBox_dropimage:
          return &static_cast<ComboBox*>(item)->dropimage;

        case Id::ComboBox_handleimage:
          return &static_cast<ComboBox*>(item)->handleimage;

        default:
          throw std::runtime_error("Invalid Sprite Id");
      }
    }

    [[gnu::unused]] const char **address_of_text(Item *item, Id id)
    {
      switch(id)
      {
        case Id::Text_text:
          return &static_cast<Text*>(item)->text;

        case Id::TextArea_text:
          return &static_cast<TextArea*>(item)->text;

        case Id::Label_text:
          return &static_cast<Label*>(item)->text;

        case Id::Field_format:
          return &static_cast<Field*>(item)->format;

        case Id::Button_text:
          return &static_cast<Button*>(item)->text;

        case Id::LineEdit_text:
          return &static_cast<LineEdit*>(item)->text;

        case Id::SlideEdit_format:
          return &static_cast<SlideEdit*>(item)->format;

        case Id::ListBox_text:
          return &static_cast<ListBox*>(item)->text;

        case Id::ComboBox_text:
          return &static_cast<ComboBox*>(item)->text;

        default:
          throw std::runtime_error("Invalid Text Id");
      }
    }

    ///////////////////////// always_bind /////////////////////////////////////
    [[gnu::unused]] bool always_bind(Id id)
    {
      switch (id)
      {
        case Ui::Id::Item_x:
        case Ui::Id::Item_y:
        case Ui::Id::Item_width:
        case Ui::Id::Item_height:
        case Ui::Id::Item_scale:
        case Ui::Id::Item_rotation:
        case Ui::Id::Item_opacity:
        case Ui::Id::Control_fontheight:
        case Ui::Id::ScrollArea_contentwidth:
        case Ui::Id::ScrollArea_contentheight:
          return true;

        default:
          return false;
      }
    }

    ///////////////////////// create_item /////////////////////////////////////
    [[gnu::unused]] Item *create_item(Context &context, Id id, Item *parent = nullptr)
    {
      switch (id)
      {
        case Ui::Id::Item:
          return context.create_item<Ui::Item>(parent);

        case Ui::Id::Rectangle:
          return context.create_item<Ui::Rectangle>(parent);

        case Ui::Id::Text:
          return context.create_item<Ui::Text>(parent);

        case Ui::Id::Image:
          return context.create_item<Ui::Image>(parent);

        case Ui::Id::Frame:
          return context.create_item<Ui::Frame>(parent);

        case Ui::Id::Row:
          return context.create_item<Ui::Row>(parent);

        case Ui::Id::Column:
          return context.create_item<Ui::Column>(parent);

        case Ui::Id::ScrollBar:
          return context.create_item<Ui::ScrollBar>(parent);

        case Ui::Id::ScrollArea:
          return context.create_item<Ui::ScrollArea>(parent);

        case Ui::Id::TextArea:
          return context.create_item<Ui::TextArea>(parent);

        case Ui::Id::Label:
          return context.create_item<Ui::Label>(parent);

        case Ui::Id::Field:
          return context.create_item<Ui::Field>(parent);

        case Ui::Id::Button:
          return context.create_item<Ui::Button>(parent);

        case Ui::Id::CheckButton:
          return context.create_item<Ui::CheckButton>(parent);

        case Ui::Id::DragButton:
          return context.create_item<Ui::DragButton>(parent);

        case Ui::Id::Slider:
          return context.create_item<Ui::Slider>(parent);

        case Ui::Id::LineEdit:
          return context.create_item<Ui::LineEdit>(parent);

        case Ui::Id::SlideEdit:
          return context.create_item<Ui::SlideEdit>(parent);

        case Ui::Id::ListBox:
          return context.create_item<Ui::ListBox>(parent);

        case Ui::Id::ComboBox:
          return context.create_item<Ui::ComboBox>(parent);

        case Ui::Id::GroupBox:
          return context.create_item<Ui::GroupBox>(parent);

        default:
          throw std::runtime_error("Invalid Item Id");
      }
    }

#ifdef DATUMUIC
    ///////////////////////// name_of //////////////////////////////////////////
    [[gnu::unused]] const char *name_of(Id id)
    {
      switch(id)
      {
        case Id::Item:
          return "Item";
        case Id::Item_x:
          return "x";
        case Id::Item_y:
          return "y";
        case Id::Item_width:
          return "width";
        case Id::Item_height:
          return "height";
        case Id::Item_z:
          return "z";
        case Id::Item_scale:
          return "scale";
        case Id::Item_rotation:
          return "rotation";
        case Id::Item_opacity:
          return "opacity";
        case Id::Item_clip:
          return "clip";
        case Id::Item_visible:
          return "visible";
        case Id::Rectangle:
          return "Rectangle";
        case Id::Rectangle_color:
          return "color";
        case Id::Rectangle_color_r:
          return "color.r";
        case Id::Rectangle_color_g:
          return "color.g";
        case Id::Rectangle_color_b:
          return "color.b";
        case Id::Rectangle_color_a:
          return "color.a";
        case Id::Rectangle_borderwidth:
          return "borderwidth";
        case Id::Rectangle_bordercolor:
          return "bordercolor";
        case Id::Rectangle_bordercolor_r:
          return "bordercolor.r";
        case Id::Rectangle_bordercolor_g:
          return "bordercolor.g";
        case Id::Rectangle_bordercolor_b:
          return "bordercolor.b";
        case Id::Rectangle_bordercolor_a:
          return "bordercolor.a";
        case Id::Text:
          return "Text";
        case Id::Text_text:
          return "text";
        case Id::Text_font:
          return "font";
        case Id::Text_color:
          return "color";
        case Id::Text_color_r:
          return "color.r";
        case Id::Text_color_g:
          return "color.g";
        case Id::Text_color_b:
          return "color.b";
        case Id::Text_color_a:
          return "color.a";
        case Id::Text_horizontalalign:
          return "horizontalalign";
        case Id::Text_verticalalign:
          return "verticalalign";
        case Id::Image:
          return "Image";
        case Id::Image_image:
          return "image";
        case Id::Image_tint:
          return "tint";
        case Id::Image_tint_r:
          return "tint.r";
        case Id::Image_tint_g:
          return "tint.g";
        case Id::Image_tint_b:
          return "tint.b";
        case Id::Image_tint_a:
          return "tint.a";
        case Id::Image_layer:
          return "layer";
        case Id::Image_fillmode:
          return "fillmode";
        case Id::Image_horizontalalign:
          return "horizontalalign";
        case Id::Image_verticalalign:
          return "verticalalign";
        case Id::Frame:
          return "Frame";
        case Id::Frame_image:
          return "image";
        case Id::Frame_tint:
          return "tint";
        case Id::Frame_tint_r:
          return "tint.r";
        case Id::Frame_tint_g:
          return "tint.g";
        case Id::Frame_tint_b:
          return "tint.b";
        case Id::Frame_tint_a:
          return "tint.a";
        case Id::Frame_layer:
          return "layer";
        case Id::Frame_slice:
          return "slice";
        case Id::Row:
          return "Row";
        case Id::Row_spacing:
          return "spacing";
        case Id::Column:
          return "Column";
        case Id::Column_spacing:
          return "spacing";
        case Id::ScrollBar:
          return "ScrollBar";
        case Id::ScrollBar_value:
          return "value";
        case Id::ScrollBar_size:
          return "size";
        case Id::ScrollBar_orientation:
          return "orientation";
        case Id::ScrollBar_handleimage:
          return "handleimage";
        case Id::ScrollBar_handletint:
          return "handletint";
        case Id::ScrollBar_handletint_r:
          return "handletint.r";
        case Id::ScrollBar_handletint_g:
          return "handletint.g";
        case Id::ScrollBar_handletint_b:
          return "handletint.b";
        case Id::ScrollBar_handletint_a:
          return "handletint.a";
        case Id::ScrollBar_handlelayer:
          return "handlelayer";
        case Id::ScrollBar_handleslice:
          return "handleslice";
        case Id::ScrollBar_handleinset:
          return "handleinset";
        case Id::ScrollBar_hovered:
          return "hovered";
        case Id::ScrollBar_pressed:
          return "pressed";
        case Id::ScrollArea:
          return "ScrollArea";
        case Id::ScrollArea_contentwidth:
          return "contentwidth";
        case Id::ScrollArea_contentheight:
          return "contentheight";
        case Id::ScrollArea_handleimage:
          return "handleimage";
        case Id::ScrollArea_handletint:
          return "handletint";
        case Id::ScrollArea_handletint_r:
          return "handletint.r";
        case Id::ScrollArea_handletint_g:
          return "handletint.g";
        case Id::ScrollArea_handletint_b:
          return "handletint.b";
        case Id::ScrollArea_handletint_a:
          return "handletint.a";
        case Id::ScrollArea_handlelayer:
          return "handlelayer";
        case Id::ScrollArea_handleslice:
          return "handleslice";
        case Id::ScrollArea_handlesize:
          return "handlesize";
        case Id::ScrollArea_handleinset:
          return "handleinset";
        case Id::ScrollArea_wheelscroll:
          return "wheelscroll";
        case Id::ScrollArea_interactive:
          return "interactive";
        case Id::TextArea:
          return "TextArea";
        case Id::TextArea_text:
          return "text";
        case Id::TextArea_font:
          return "font";
        case Id::TextArea_textcolor:
          return "textcolor";
        case Id::TextArea_textcolor_r:
          return "textcolor.r";
        case Id::TextArea_textcolor_g:
          return "textcolor.g";
        case Id::TextArea_textcolor_b:
          return "textcolor.b";
        case Id::TextArea_textcolor_a:
          return "textcolor.a";
        case Id::TextArea_horizontalalign:
          return "horizontalalign";
        case Id::TextArea_verticalalign:
          return "verticalalign";
        case Id::TextArea_selectioncolor:
          return "selectioncolor";
        case Id::TextArea_selectioncolor_r:
          return "selectioncolor.r";
        case Id::TextArea_selectioncolor_g:
          return "selectioncolor.g";
        case Id::TextArea_selectioncolor_b:
          return "selectioncolor.b";
        case Id::TextArea_selectioncolor_a:
          return "selectioncolor.a";
        case Id::TextArea_selectiontextcolor:
          return "selectiontextcolor";
        case Id::TextArea_selectiontextcolor_r:
          return "selectiontextcolor.r";
        case Id::TextArea_selectiontextcolor_g:
          return "selectiontextcolor.g";
        case Id::TextArea_selectiontextcolor_b:
          return "selectiontextcolor.b";
        case Id::TextArea_selectiontextcolor_a:
          return "selectiontextcolor.a";
        case Id::TextArea_interactive:
          return "interactive";
        case Id::Control:
          return "Control";
        case Id::Control_borderwidth:
          return "borderwidth";
        case Id::Control_bordercolor:
          return "bordercolor";
        case Id::Control_bordercolor_r:
          return "bordercolor.r";
        case Id::Control_bordercolor_g:
          return "bordercolor.g";
        case Id::Control_bordercolor_b:
          return "bordercolor.b";
        case Id::Control_bordercolor_a:
          return "bordercolor.a";
        case Id::Control_backgroundimage:
          return "backgroundimage";
        case Id::Control_backgroundcolor:
          return "backgroundcolor";
        case Id::Control_backgroundcolor_r:
          return "backgroundcolor.r";
        case Id::Control_backgroundcolor_g:
          return "backgroundcolor.g";
        case Id::Control_backgroundcolor_b:
          return "backgroundcolor.b";
        case Id::Control_backgroundcolor_a:
          return "backgroundcolor.a";
        case Id::Control_backgroundlayer:
          return "backgroundlayer";
        case Id::Control_backgroundslice:
          return "backgroundslice";
        case Id::Control_font:
          return "font";
        case Id::Control_fontheight:
          return "fontheight";
        case Id::Control_enabled:
          return "enabled";
        case Id::Control_hovered:
          return "hovered";
        case Id::Control_action:
          return "action";
        case Id::Label:
          return "Label";
        case Id::Label_text:
          return "text";
        case Id::Label_textcolor:
          return "textcolor";
        case Id::Label_textcolor_r:
          return "textcolor.r";
        case Id::Label_textcolor_g:
          return "textcolor.g";
        case Id::Label_textcolor_b:
          return "textcolor.b";
        case Id::Label_textcolor_a:
          return "textcolor.a";
        case Id::Label_horizontalalign:
          return "horizontalalign";
        case Id::Label_verticalalign:
          return "verticalalign";
        case Id::Label_padding:
          return "padding";
        case Id::Field:
          return "Field";
        case Id::Field_value:
          return "value";
        case Id::Field_precision:
          return "precision";
        case Id::Field_format:
          return "format";
        case Id::Field_textcolor:
          return "textcolor";
        case Id::Field_textcolor_r:
          return "textcolor.r";
        case Id::Field_textcolor_g:
          return "textcolor.g";
        case Id::Field_textcolor_b:
          return "textcolor.b";
        case Id::Field_textcolor_a:
          return "textcolor.a";
        case Id::Field_horizontalalign:
          return "horizontalalign";
        case Id::Field_verticalalign:
          return "verticalalign";
        case Id::Field_padding:
          return "padding";
        case Id::Button:
          return "Button";
        case Id::Button_text:
          return "text";
        case Id::Button_textcolor:
          return "textcolor";
        case Id::Button_textcolor_r:
          return "textcolor.r";
        case Id::Button_textcolor_g:
          return "textcolor.g";
        case Id::Button_textcolor_b:
          return "textcolor.b";
        case Id::Button_textcolor_a:
          return "textcolor.a";
        case Id::Button_textalign:
          return "textalign";
        case Id::Button_image:
          return "image";
        case Id::Button_imagetint:
          return "imagetint";
        case Id::Button_imagetint_r:
          return "imagetint.r";
        case Id::Button_imagetint_g:
          return "imagetint.g";
        case Id::Button_imagetint_b:
          return "imagetint.b";
        case Id::Button_imagetint_a:
          return "imagetint.a";
        case Id::Button_imagelayer:
          return "imagelayer";
        case Id::Button_imagealign:
          return "imagealign";
        case Id::Button_spacing:
          return "spacing";
        case Id::Button_padding:
          return "padding";
        case Id::Button_pressed:
          return "pressed";
        case Id::CheckButton:
          return "CheckButton";
        case Id::CheckButton_checked:
          return "checked";
        case Id::CheckButton_innercolor:
          return "innercolor";
        case Id::CheckButton_innercolor_r:
          return "innercolor.r";
        case Id::CheckButton_innercolor_g:
          return "innercolor.g";
        case Id::CheckButton_innercolor_b:
          return "innercolor.b";
        case Id::CheckButton_innercolor_a:
          return "innercolor.a";
        case Id::CheckButton_outercolor:
          return "outercolor";
        case Id::CheckButton_outercolor_r:
          return "outercolor.r";
        case Id::CheckButton_outercolor_g:
          return "outercolor.g";
        case Id::CheckButton_outercolor_b:
          return "outercolor.b";
        case Id::CheckButton_outercolor_a:
          return "outercolor.a";
        case Id::DragButton:
          return "DragButton";
        case Id::DragButton_dx:
          return "dx";
        case Id::DragButton_dy:
          return "dy";
        case Id::Slider:
          return "Slider";
        case Id::Slider_value:
          return "value";
        case Id::Slider_minvalue:
          return "minvalue";
        case Id::Slider_maxvalue:
          return "maxvalue";
        case Id::Slider_orientation:
          return "orientation";
        case Id::Slider_inset:
          return "inset";
        case Id::Slider_handleimage:
          return "handleimage";
        case Id::Slider_handletint:
          return "handletint";
        case Id::Slider_handletint_r:
          return "handletint.r";
        case Id::Slider_handletint_g:
          return "handletint.g";
        case Id::Slider_handletint_b:
          return "handletint.b";
        case Id::Slider_handletint_a:
          return "handletint.a";
        case Id::Slider_handlelayer:
          return "handlelayer";
        case Id::Slider_pressed:
          return "pressed";
        case Id::LineEdit:
          return "LineEdit";
        case Id::LineEdit_text:
          return "text";
        case Id::LineEdit_textcolor:
          return "textcolor";
        case Id::LineEdit_textcolor_r:
          return "textcolor.r";
        case Id::LineEdit_textcolor_g:
          return "textcolor.g";
        case Id::LineEdit_textcolor_b:
          return "textcolor.b";
        case Id::LineEdit_textcolor_a:
          return "textcolor.a";
        case Id::LineEdit_padding:
          return "padding";
        case Id::LineEdit_selectioncolor:
          return "selectioncolor";
        case Id::LineEdit_selectioncolor_r:
          return "selectioncolor.r";
        case Id::LineEdit_selectioncolor_g:
          return "selectioncolor.g";
        case Id::LineEdit_selectioncolor_b:
          return "selectioncolor.b";
        case Id::LineEdit_selectioncolor_a:
          return "selectioncolor.a";
        case Id::LineEdit_selectiontextcolor:
          return "selectiontextcolor";
        case Id::LineEdit_selectiontextcolor_r:
          return "selectiontextcolor.r";
        case Id::LineEdit_selectiontextcolor_g:
          return "selectiontextcolor.g";
        case Id::LineEdit_selectiontextcolor_b:
          return "selectiontextcolor.b";
        case Id::LineEdit_selectiontextcolor_a:
          return "selectiontextcolor.a";
        case Id::LineEdit_focused:
          return "focused";
        case Id::LineEdit_filter:
          return "filter";
        case Id::SlideEdit:
          return "SlideEdit";
        case Id::SlideEdit_value:
          return "value";
        case Id::SlideEdit_precision:
          return "precision";
        case Id::SlideEdit_format:
          return "format";
        case Id::SlideEdit_minvalue:
          return "minvalue";
        case Id::SlideEdit_maxvalue:
          return "maxvalue";
        case Id::SlideEdit_handleimage:
          return "handleimage";
        case Id::SlideEdit_handleimagetint:
          return "handleimagetint";
        case Id::SlideEdit_handleimagetint_r:
          return "handleimagetint.r";
        case Id::SlideEdit_handleimagetint_g:
          return "handleimagetint.g";
        case Id::SlideEdit_handleimagetint_b:
          return "handleimagetint.b";
        case Id::SlideEdit_handleimagetint_a:
          return "handleimagetint.a";
        case Id::SlideEdit_handleimagelayer:
          return "handleimagelayer";
        case Id::SlideEdit_sliderate:
          return "sliderate";
        case Id::ListBox:
          return "ListBox";
        case Id::ListBox_text:
          return "text";
        case Id::ListBox_textcolor:
          return "textcolor";
        case Id::ListBox_textcolor_r:
          return "textcolor.r";
        case Id::ListBox_textcolor_g:
          return "textcolor.g";
        case Id::ListBox_textcolor_b:
          return "textcolor.b";
        case Id::ListBox_textcolor_a:
          return "textcolor.a";
        case Id::ListBox_handleimage:
          return "handleimage";
        case Id::ListBox_handletint:
          return "handletint";
        case Id::ListBox_handletint_r:
          return "handletint.r";
        case Id::ListBox_handletint_g:
          return "handletint.g";
        case Id::ListBox_handletint_b:
          return "handletint.b";
        case Id::ListBox_handletint_a:
          return "handletint.a";
        case Id::ListBox_handlelayer:
          return "handlelayer";
        case Id::ListBox_handleslice:
          return "handleslice";
        case Id::ListBox_handlesize:
          return "handlesize";
        case Id::ListBox_handleinset:
          return "handleinset";
        case Id::ListBox_padding:
          return "padding";
        case Id::ListBox_selectioncolor:
          return "selectioncolor";
        case Id::ListBox_selectioncolor_r:
          return "selectioncolor.r";
        case Id::ListBox_selectioncolor_g:
          return "selectioncolor.g";
        case Id::ListBox_selectioncolor_b:
          return "selectioncolor.b";
        case Id::ListBox_selectioncolor_a:
          return "selectioncolor.a";
        case Id::ListBox_selectiontextcolor:
          return "selectiontextcolor";
        case Id::ListBox_selectiontextcolor_r:
          return "selectiontextcolor.r";
        case Id::ListBox_selectiontextcolor_g:
          return "selectiontextcolor.g";
        case Id::ListBox_selectiontextcolor_b:
          return "selectiontextcolor.b";
        case Id::ListBox_selectiontextcolor_a:
          return "selectiontextcolor.a";
        case Id::ListBox_focused:
          return "focused";
        case Id::ComboBox:
          return "ComboBox";
        case Id::ComboBox_text:
          return "text";
        case Id::ComboBox_textcolor:
          return "textcolor";
        case Id::ComboBox_textcolor_r:
          return "textcolor.r";
        case Id::ComboBox_textcolor_g:
          return "textcolor.g";
        case Id::ComboBox_textcolor_b:
          return "textcolor.b";
        case Id::ComboBox_textcolor_a:
          return "textcolor.a";
        case Id::ComboBox_dropimage:
          return "dropimage";
        case Id::ComboBox_dropimagetint:
          return "dropimagetint";
        case Id::ComboBox_dropimagetint_r:
          return "dropimagetint.r";
        case Id::ComboBox_dropimagetint_g:
          return "dropimagetint.g";
        case Id::ComboBox_dropimagetint_b:
          return "dropimagetint.b";
        case Id::ComboBox_dropimagetint_a:
          return "dropimagetint.a";
        case Id::ComboBox_dropimagelayer:
          return "dropimagelayer";
        case Id::ComboBox_listborderwidth:
          return "listborderwidth";
        case Id::ComboBox_listbordercolor:
          return "listbordercolor";
        case Id::ComboBox_listbordercolor_r:
          return "listbordercolor.r";
        case Id::ComboBox_listbordercolor_g:
          return "listbordercolor.g";
        case Id::ComboBox_listbordercolor_b:
          return "listbordercolor.b";
        case Id::ComboBox_listbordercolor_a:
          return "listbordercolor.a";
        case Id::ComboBox_listbackgroundcolor:
          return "listbackgroundcolor";
        case Id::ComboBox_listbackgroundcolor_r:
          return "listbackgroundcolor.r";
        case Id::ComboBox_listbackgroundcolor_g:
          return "listbackgroundcolor.g";
        case Id::ComboBox_listbackgroundcolor_b:
          return "listbackgroundcolor.b";
        case Id::ComboBox_listbackgroundcolor_a:
          return "listbackgroundcolor.a";
        case Id::ComboBox_handleimage:
          return "handleimage";
        case Id::ComboBox_handletint:
          return "handletint";
        case Id::ComboBox_handletint_r:
          return "handletint.r";
        case Id::ComboBox_handletint_g:
          return "handletint.g";
        case Id::ComboBox_handletint_b:
          return "handletint.b";
        case Id::ComboBox_handletint_a:
          return "handletint.a";
        case Id::ComboBox_handlelayer:
          return "handlelayer";
        case Id::ComboBox_handleslice:
          return "handleslice";
        case Id::ComboBox_handlesize:
          return "handlesize";
        case Id::ComboBox_handleinset:
          return "handleinset";
        case Id::ComboBox_padding:
          return "padding";
        case Id::ComboBox_selectioncolor:
          return "selectioncolor";
        case Id::ComboBox_selectioncolor_r:
          return "selectioncolor.r";
        case Id::ComboBox_selectioncolor_g:
          return "selectioncolor.g";
        case Id::ComboBox_selectioncolor_b:
          return "selectioncolor.b";
        case Id::ComboBox_selectioncolor_a:
          return "selectioncolor.a";
        case Id::ComboBox_selectiontextcolor:
          return "selectiontextcolor";
        case Id::ComboBox_selectiontextcolor_r:
          return "selectiontextcolor.r";
        case Id::ComboBox_selectiontextcolor_g:
          return "selectiontextcolor.g";
        case Id::ComboBox_selectiontextcolor_b:
          return "selectiontextcolor.b";
        case Id::ComboBox_selectiontextcolor_a:
          return "selectiontextcolor.a";
        case Id::ComboBox_pressed:
          return "pressed";
        case Id::ComboBox_dropped:
          return "dropped";
        case Id::GroupBox:
          return "GroupBox";
      }

      throw std::runtime_error("Unknown Type");
    }

    ///////////////////////// init_ids_map ////////////////////////////////////
    [[gnu::unused]] void init_ids_map(std::unordered_map<std::string, Id> &ids)
    {
      ids["Item"] = Id::Item;
      ids["Item.x"] = Id::Item_x;
      ids["Item.y"] = Id::Item_y;
      ids["Item.width"] = Id::Item_width;
      ids["Item.height"] = Id::Item_height;
      ids["Item.z"] = Id::Item_z;
      ids["Item.scale"] = Id::Item_scale;
      ids["Item.rotation"] = Id::Item_rotation;
      ids["Item.opacity"] = Id::Item_opacity;
      ids["Item.clip"] = Id::Item_clip;
      ids["Item.visible"] = Id::Item_visible;
      ids["Rectangle"] = Id::Rectangle;
      ids["Rectangle.x"] = Id::Item_x;
      ids["Rectangle.y"] = Id::Item_y;
      ids["Rectangle.width"] = Id::Item_width;
      ids["Rectangle.height"] = Id::Item_height;
      ids["Rectangle.z"] = Id::Item_z;
      ids["Rectangle.scale"] = Id::Item_scale;
      ids["Rectangle.rotation"] = Id::Item_rotation;
      ids["Rectangle.opacity"] = Id::Item_opacity;
      ids["Rectangle.clip"] = Id::Item_clip;
      ids["Rectangle.visible"] = Id::Item_visible;
      ids["Rectangle.color"] = Id::Rectangle_color;
      ids["Rectangle.color.r"] = Id::Rectangle_color_r;
      ids["Rectangle.color.g"] = Id::Rectangle_color_g;
      ids["Rectangle.color.b"] = Id::Rectangle_color_b;
      ids["Rectangle.color.a"] = Id::Rectangle_color_a;
      ids["Rectangle.borderwidth"] = Id::Rectangle_borderwidth;
      ids["Rectangle.bordercolor"] = Id::Rectangle_bordercolor;
      ids["Rectangle.bordercolor.r"] = Id::Rectangle_bordercolor_r;
      ids["Rectangle.bordercolor.g"] = Id::Rectangle_bordercolor_g;
      ids["Rectangle.bordercolor.b"] = Id::Rectangle_bordercolor_b;
      ids["Rectangle.bordercolor.a"] = Id::Rectangle_bordercolor_a;
      ids["Text"] = Id::Text;
      ids["Text.x"] = Id::Item_x;
      ids["Text.y"] = Id::Item_y;
      ids["Text.width"] = Id::Item_width;
      ids["Text.height"] = Id::Item_height;
      ids["Text.z"] = Id::Item_z;
      ids["Text.scale"] = Id::Item_scale;
      ids["Text.rotation"] = Id::Item_rotation;
      ids["Text.opacity"] = Id::Item_opacity;
      ids["Text.clip"] = Id::Item_clip;
      ids["Text.visible"] = Id::Item_visible;
      ids["Text.text"] = Id::Text_text;
      ids["Text.font"] = Id::Text_font;
      ids["Text.color"] = Id::Text_color;
      ids["Text.color.r"] = Id::Text_color_r;
      ids["Text.color.g"] = Id::Text_color_g;
      ids["Text.color.b"] = Id::Text_color_b;
      ids["Text.color.a"] = Id::Text_color_a;
      ids["Text.horizontalalign"] = Id::Text_horizontalalign;
      ids["Text.verticalalign"] = Id::Text_verticalalign;
      ids["Image"] = Id::Image;
      ids["Image.x"] = Id::Item_x;
      ids["Image.y"] = Id::Item_y;
      ids["Image.width"] = Id::Item_width;
      ids["Image.height"] = Id::Item_height;
      ids["Image.z"] = Id::Item_z;
      ids["Image.scale"] = Id::Item_scale;
      ids["Image.rotation"] = Id::Item_rotation;
      ids["Image.opacity"] = Id::Item_opacity;
      ids["Image.clip"] = Id::Item_clip;
      ids["Image.visible"] = Id::Item_visible;
      ids["Image.image"] = Id::Image_image;
      ids["Image.tint"] = Id::Image_tint;
      ids["Image.tint.r"] = Id::Image_tint_r;
      ids["Image.tint.g"] = Id::Image_tint_g;
      ids["Image.tint.b"] = Id::Image_tint_b;
      ids["Image.tint.a"] = Id::Image_tint_a;
      ids["Image.layer"] = Id::Image_layer;
      ids["Image.fillmode"] = Id::Image_fillmode;
      ids["Image.horizontalalign"] = Id::Image_horizontalalign;
      ids["Image.verticalalign"] = Id::Image_verticalalign;
      ids["Frame"] = Id::Frame;
      ids["Frame.x"] = Id::Item_x;
      ids["Frame.y"] = Id::Item_y;
      ids["Frame.width"] = Id::Item_width;
      ids["Frame.height"] = Id::Item_height;
      ids["Frame.z"] = Id::Item_z;
      ids["Frame.scale"] = Id::Item_scale;
      ids["Frame.rotation"] = Id::Item_rotation;
      ids["Frame.opacity"] = Id::Item_opacity;
      ids["Frame.clip"] = Id::Item_clip;
      ids["Frame.visible"] = Id::Item_visible;
      ids["Frame.image"] = Id::Frame_image;
      ids["Frame.tint"] = Id::Frame_tint;
      ids["Frame.tint.r"] = Id::Frame_tint_r;
      ids["Frame.tint.g"] = Id::Frame_tint_g;
      ids["Frame.tint.b"] = Id::Frame_tint_b;
      ids["Frame.tint.a"] = Id::Frame_tint_a;
      ids["Frame.layer"] = Id::Frame_layer;
      ids["Frame.slice"] = Id::Frame_slice;
      ids["Row"] = Id::Row;
      ids["Row.x"] = Id::Item_x;
      ids["Row.y"] = Id::Item_y;
      ids["Row.width"] = Id::Item_width;
      ids["Row.height"] = Id::Item_height;
      ids["Row.z"] = Id::Item_z;
      ids["Row.scale"] = Id::Item_scale;
      ids["Row.rotation"] = Id::Item_rotation;
      ids["Row.opacity"] = Id::Item_opacity;
      ids["Row.clip"] = Id::Item_clip;
      ids["Row.visible"] = Id::Item_visible;
      ids["Row.spacing"] = Id::Row_spacing;
      ids["Column"] = Id::Column;
      ids["Column.x"] = Id::Item_x;
      ids["Column.y"] = Id::Item_y;
      ids["Column.width"] = Id::Item_width;
      ids["Column.height"] = Id::Item_height;
      ids["Column.z"] = Id::Item_z;
      ids["Column.scale"] = Id::Item_scale;
      ids["Column.rotation"] = Id::Item_rotation;
      ids["Column.opacity"] = Id::Item_opacity;
      ids["Column.clip"] = Id::Item_clip;
      ids["Column.visible"] = Id::Item_visible;
      ids["Column.spacing"] = Id::Column_spacing;
      ids["ScrollBar"] = Id::ScrollBar;
      ids["ScrollBar.x"] = Id::Item_x;
      ids["ScrollBar.y"] = Id::Item_y;
      ids["ScrollBar.width"] = Id::Item_width;
      ids["ScrollBar.height"] = Id::Item_height;
      ids["ScrollBar.z"] = Id::Item_z;
      ids["ScrollBar.scale"] = Id::Item_scale;
      ids["ScrollBar.rotation"] = Id::Item_rotation;
      ids["ScrollBar.opacity"] = Id::Item_opacity;
      ids["ScrollBar.clip"] = Id::Item_clip;
      ids["ScrollBar.visible"] = Id::Item_visible;
      ids["ScrollBar.value"] = Id::ScrollBar_value;
      ids["ScrollBar.size"] = Id::ScrollBar_size;
      ids["ScrollBar.orientation"] = Id::ScrollBar_orientation;
      ids["ScrollBar.handleimage"] = Id::ScrollBar_handleimage;
      ids["ScrollBar.handletint"] = Id::ScrollBar_handletint;
      ids["ScrollBar.handletint.r"] = Id::ScrollBar_handletint_r;
      ids["ScrollBar.handletint.g"] = Id::ScrollBar_handletint_g;
      ids["ScrollBar.handletint.b"] = Id::ScrollBar_handletint_b;
      ids["ScrollBar.handletint.a"] = Id::ScrollBar_handletint_a;
      ids["ScrollBar.handlelayer"] = Id::ScrollBar_handlelayer;
      ids["ScrollBar.handleslice"] = Id::ScrollBar_handleslice;
      ids["ScrollBar.handleinset"] = Id::ScrollBar_handleinset;
      ids["ScrollBar.hovered"] = Id::ScrollBar_hovered;
      ids["ScrollBar.pressed"] = Id::ScrollBar_pressed;
      ids["ScrollArea"] = Id::ScrollArea;
      ids["ScrollArea.x"] = Id::Item_x;
      ids["ScrollArea.y"] = Id::Item_y;
      ids["ScrollArea.width"] = Id::Item_width;
      ids["ScrollArea.height"] = Id::Item_height;
      ids["ScrollArea.z"] = Id::Item_z;
      ids["ScrollArea.scale"] = Id::Item_scale;
      ids["ScrollArea.rotation"] = Id::Item_rotation;
      ids["ScrollArea.opacity"] = Id::Item_opacity;
      ids["ScrollArea.clip"] = Id::Item_clip;
      ids["ScrollArea.visible"] = Id::Item_visible;
      ids["ScrollArea.contentwidth"] = Id::ScrollArea_contentwidth;
      ids["ScrollArea.contentheight"] = Id::ScrollArea_contentheight;
      ids["ScrollArea.handleimage"] = Id::ScrollArea_handleimage;
      ids["ScrollArea.handletint"] = Id::ScrollArea_handletint;
      ids["ScrollArea.handletint.r"] = Id::ScrollArea_handletint_r;
      ids["ScrollArea.handletint.g"] = Id::ScrollArea_handletint_g;
      ids["ScrollArea.handletint.b"] = Id::ScrollArea_handletint_b;
      ids["ScrollArea.handletint.a"] = Id::ScrollArea_handletint_a;
      ids["ScrollArea.handlelayer"] = Id::ScrollArea_handlelayer;
      ids["ScrollArea.handleslice"] = Id::ScrollArea_handleslice;
      ids["ScrollArea.handlesize"] = Id::ScrollArea_handlesize;
      ids["ScrollArea.handleinset"] = Id::ScrollArea_handleinset;
      ids["ScrollArea.wheelscroll"] = Id::ScrollArea_wheelscroll;
      ids["ScrollArea.interactive"] = Id::ScrollArea_interactive;
      ids["TextArea"] = Id::TextArea;
      ids["TextArea.x"] = Id::Item_x;
      ids["TextArea.y"] = Id::Item_y;
      ids["TextArea.width"] = Id::Item_width;
      ids["TextArea.height"] = Id::Item_height;
      ids["TextArea.z"] = Id::Item_z;
      ids["TextArea.scale"] = Id::Item_scale;
      ids["TextArea.rotation"] = Id::Item_rotation;
      ids["TextArea.opacity"] = Id::Item_opacity;
      ids["TextArea.clip"] = Id::Item_clip;
      ids["TextArea.visible"] = Id::Item_visible;
      ids["TextArea.text"] = Id::TextArea_text;
      ids["TextArea.font"] = Id::TextArea_font;
      ids["TextArea.textcolor"] = Id::TextArea_textcolor;
      ids["TextArea.textcolor.r"] = Id::TextArea_textcolor_r;
      ids["TextArea.textcolor.g"] = Id::TextArea_textcolor_g;
      ids["TextArea.textcolor.b"] = Id::TextArea_textcolor_b;
      ids["TextArea.textcolor.a"] = Id::TextArea_textcolor_a;
      ids["TextArea.horizontalalign"] = Id::TextArea_horizontalalign;
      ids["TextArea.verticalalign"] = Id::TextArea_verticalalign;
      ids["TextArea.selectioncolor"] = Id::TextArea_selectioncolor;
      ids["TextArea.selectioncolor.r"] = Id::TextArea_selectioncolor_r;
      ids["TextArea.selectioncolor.g"] = Id::TextArea_selectioncolor_g;
      ids["TextArea.selectioncolor.b"] = Id::TextArea_selectioncolor_b;
      ids["TextArea.selectioncolor.a"] = Id::TextArea_selectioncolor_a;
      ids["TextArea.selectiontextcolor"] = Id::TextArea_selectiontextcolor;
      ids["TextArea.selectiontextcolor.r"] = Id::TextArea_selectiontextcolor_r;
      ids["TextArea.selectiontextcolor.g"] = Id::TextArea_selectiontextcolor_g;
      ids["TextArea.selectiontextcolor.b"] = Id::TextArea_selectiontextcolor_b;
      ids["TextArea.selectiontextcolor.a"] = Id::TextArea_selectiontextcolor_a;
      ids["TextArea.interactive"] = Id::TextArea_interactive;
      ids["Control"] = Id::Control;
      ids["Control.x"] = Id::Item_x;
      ids["Control.y"] = Id::Item_y;
      ids["Control.width"] = Id::Item_width;
      ids["Control.height"] = Id::Item_height;
      ids["Control.z"] = Id::Item_z;
      ids["Control.scale"] = Id::Item_scale;
      ids["Control.rotation"] = Id::Item_rotation;
      ids["Control.opacity"] = Id::Item_opacity;
      ids["Control.clip"] = Id::Item_clip;
      ids["Control.visible"] = Id::Item_visible;
      ids["Control.borderwidth"] = Id::Control_borderwidth;
      ids["Control.bordercolor"] = Id::Control_bordercolor;
      ids["Control.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["Control.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["Control.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["Control.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["Control.backgroundimage"] = Id::Control_backgroundimage;
      ids["Control.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["Control.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["Control.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["Control.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["Control.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["Control.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["Control.backgroundslice"] = Id::Control_backgroundslice;
      ids["Control.font"] = Id::Control_font;
      ids["Control.fontheight"] = Id::Control_fontheight;
      ids["Control.enabled"] = Id::Control_enabled;
      ids["Control.hovered"] = Id::Control_hovered;
      ids["Control.action"] = Id::Control_action;
      ids["Label"] = Id::Label;
      ids["Label.x"] = Id::Item_x;
      ids["Label.y"] = Id::Item_y;
      ids["Label.width"] = Id::Item_width;
      ids["Label.height"] = Id::Item_height;
      ids["Label.z"] = Id::Item_z;
      ids["Label.scale"] = Id::Item_scale;
      ids["Label.rotation"] = Id::Item_rotation;
      ids["Label.opacity"] = Id::Item_opacity;
      ids["Label.clip"] = Id::Item_clip;
      ids["Label.visible"] = Id::Item_visible;
      ids["Label.borderwidth"] = Id::Control_borderwidth;
      ids["Label.bordercolor"] = Id::Control_bordercolor;
      ids["Label.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["Label.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["Label.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["Label.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["Label.backgroundimage"] = Id::Control_backgroundimage;
      ids["Label.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["Label.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["Label.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["Label.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["Label.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["Label.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["Label.backgroundslice"] = Id::Control_backgroundslice;
      ids["Label.font"] = Id::Control_font;
      ids["Label.fontheight"] = Id::Control_fontheight;
      ids["Label.enabled"] = Id::Control_enabled;
      ids["Label.hovered"] = Id::Control_hovered;
      ids["Label.action"] = Id::Control_action;
      ids["Label.text"] = Id::Label_text;
      ids["Label.textcolor"] = Id::Label_textcolor;
      ids["Label.textcolor.r"] = Id::Label_textcolor_r;
      ids["Label.textcolor.g"] = Id::Label_textcolor_g;
      ids["Label.textcolor.b"] = Id::Label_textcolor_b;
      ids["Label.textcolor.a"] = Id::Label_textcolor_a;
      ids["Label.horizontalalign"] = Id::Label_horizontalalign;
      ids["Label.verticalalign"] = Id::Label_verticalalign;
      ids["Label.padding"] = Id::Label_padding;
      ids["Field"] = Id::Field;
      ids["Field.x"] = Id::Item_x;
      ids["Field.y"] = Id::Item_y;
      ids["Field.width"] = Id::Item_width;
      ids["Field.height"] = Id::Item_height;
      ids["Field.z"] = Id::Item_z;
      ids["Field.scale"] = Id::Item_scale;
      ids["Field.rotation"] = Id::Item_rotation;
      ids["Field.opacity"] = Id::Item_opacity;
      ids["Field.clip"] = Id::Item_clip;
      ids["Field.visible"] = Id::Item_visible;
      ids["Field.borderwidth"] = Id::Control_borderwidth;
      ids["Field.bordercolor"] = Id::Control_bordercolor;
      ids["Field.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["Field.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["Field.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["Field.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["Field.backgroundimage"] = Id::Control_backgroundimage;
      ids["Field.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["Field.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["Field.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["Field.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["Field.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["Field.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["Field.backgroundslice"] = Id::Control_backgroundslice;
      ids["Field.font"] = Id::Control_font;
      ids["Field.fontheight"] = Id::Control_fontheight;
      ids["Field.enabled"] = Id::Control_enabled;
      ids["Field.hovered"] = Id::Control_hovered;
      ids["Field.action"] = Id::Control_action;
      ids["Field.value"] = Id::Field_value;
      ids["Field.precision"] = Id::Field_precision;
      ids["Field.format"] = Id::Field_format;
      ids["Field.textcolor"] = Id::Field_textcolor;
      ids["Field.textcolor.r"] = Id::Field_textcolor_r;
      ids["Field.textcolor.g"] = Id::Field_textcolor_g;
      ids["Field.textcolor.b"] = Id::Field_textcolor_b;
      ids["Field.textcolor.a"] = Id::Field_textcolor_a;
      ids["Field.horizontalalign"] = Id::Field_horizontalalign;
      ids["Field.verticalalign"] = Id::Field_verticalalign;
      ids["Field.padding"] = Id::Field_padding;
      ids["Button"] = Id::Button;
      ids["Button.x"] = Id::Item_x;
      ids["Button.y"] = Id::Item_y;
      ids["Button.width"] = Id::Item_width;
      ids["Button.height"] = Id::Item_height;
      ids["Button.z"] = Id::Item_z;
      ids["Button.scale"] = Id::Item_scale;
      ids["Button.rotation"] = Id::Item_rotation;
      ids["Button.opacity"] = Id::Item_opacity;
      ids["Button.clip"] = Id::Item_clip;
      ids["Button.visible"] = Id::Item_visible;
      ids["Button.borderwidth"] = Id::Control_borderwidth;
      ids["Button.bordercolor"] = Id::Control_bordercolor;
      ids["Button.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["Button.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["Button.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["Button.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["Button.backgroundimage"] = Id::Control_backgroundimage;
      ids["Button.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["Button.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["Button.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["Button.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["Button.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["Button.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["Button.backgroundslice"] = Id::Control_backgroundslice;
      ids["Button.font"] = Id::Control_font;
      ids["Button.fontheight"] = Id::Control_fontheight;
      ids["Button.enabled"] = Id::Control_enabled;
      ids["Button.hovered"] = Id::Control_hovered;
      ids["Button.action"] = Id::Control_action;
      ids["Button.text"] = Id::Button_text;
      ids["Button.textcolor"] = Id::Button_textcolor;
      ids["Button.textcolor.r"] = Id::Button_textcolor_r;
      ids["Button.textcolor.g"] = Id::Button_textcolor_g;
      ids["Button.textcolor.b"] = Id::Button_textcolor_b;
      ids["Button.textcolor.a"] = Id::Button_textcolor_a;
      ids["Button.textalign"] = Id::Button_textalign;
      ids["Button.image"] = Id::Button_image;
      ids["Button.imagetint"] = Id::Button_imagetint;
      ids["Button.imagetint.r"] = Id::Button_imagetint_r;
      ids["Button.imagetint.g"] = Id::Button_imagetint_g;
      ids["Button.imagetint.b"] = Id::Button_imagetint_b;
      ids["Button.imagetint.a"] = Id::Button_imagetint_a;
      ids["Button.imagelayer"] = Id::Button_imagelayer;
      ids["Button.imagealign"] = Id::Button_imagealign;
      ids["Button.spacing"] = Id::Button_spacing;
      ids["Button.padding"] = Id::Button_padding;
      ids["Button.pressed"] = Id::Button_pressed;
      ids["CheckButton"] = Id::CheckButton;
      ids["CheckButton.x"] = Id::Item_x;
      ids["CheckButton.y"] = Id::Item_y;
      ids["CheckButton.width"] = Id::Item_width;
      ids["CheckButton.height"] = Id::Item_height;
      ids["CheckButton.z"] = Id::Item_z;
      ids["CheckButton.scale"] = Id::Item_scale;
      ids["CheckButton.rotation"] = Id::Item_rotation;
      ids["CheckButton.opacity"] = Id::Item_opacity;
      ids["CheckButton.clip"] = Id::Item_clip;
      ids["CheckButton.visible"] = Id::Item_visible;
      ids["CheckButton.borderwidth"] = Id::Control_borderwidth;
      ids["CheckButton.bordercolor"] = Id::Control_bordercolor;
      ids["CheckButton.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["CheckButton.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["CheckButton.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["CheckButton.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["CheckButton.backgroundimage"] = Id::Control_backgroundimage;
      ids["CheckButton.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["CheckButton.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["CheckButton.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["CheckButton.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["CheckButton.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["CheckButton.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["CheckButton.backgroundslice"] = Id::Control_backgroundslice;
      ids["CheckButton.font"] = Id::Control_font;
      ids["CheckButton.fontheight"] = Id::Control_fontheight;
      ids["CheckButton.enabled"] = Id::Control_enabled;
      ids["CheckButton.hovered"] = Id::Control_hovered;
      ids["CheckButton.action"] = Id::Control_action;
      ids["CheckButton.text"] = Id::Button_text;
      ids["CheckButton.textcolor"] = Id::Button_textcolor;
      ids["CheckButton.textcolor.r"] = Id::Button_textcolor_r;
      ids["CheckButton.textcolor.g"] = Id::Button_textcolor_g;
      ids["CheckButton.textcolor.b"] = Id::Button_textcolor_b;
      ids["CheckButton.textcolor.a"] = Id::Button_textcolor_a;
      ids["CheckButton.textalign"] = Id::Button_textalign;
      ids["CheckButton.image"] = Id::Button_image;
      ids["CheckButton.imagetint"] = Id::Button_imagetint;
      ids["CheckButton.imagetint.r"] = Id::Button_imagetint_r;
      ids["CheckButton.imagetint.g"] = Id::Button_imagetint_g;
      ids["CheckButton.imagetint.b"] = Id::Button_imagetint_b;
      ids["CheckButton.imagetint.a"] = Id::Button_imagetint_a;
      ids["CheckButton.imagelayer"] = Id::Button_imagelayer;
      ids["CheckButton.imagealign"] = Id::Button_imagealign;
      ids["CheckButton.spacing"] = Id::Button_spacing;
      ids["CheckButton.padding"] = Id::Button_padding;
      ids["CheckButton.pressed"] = Id::Button_pressed;
      ids["CheckButton.checked"] = Id::CheckButton_checked;
      ids["CheckButton.innercolor"] = Id::CheckButton_innercolor;
      ids["CheckButton.innercolor.r"] = Id::CheckButton_innercolor_r;
      ids["CheckButton.innercolor.g"] = Id::CheckButton_innercolor_g;
      ids["CheckButton.innercolor.b"] = Id::CheckButton_innercolor_b;
      ids["CheckButton.innercolor.a"] = Id::CheckButton_innercolor_a;
      ids["CheckButton.outercolor"] = Id::CheckButton_outercolor;
      ids["CheckButton.outercolor.r"] = Id::CheckButton_outercolor_r;
      ids["CheckButton.outercolor.g"] = Id::CheckButton_outercolor_g;
      ids["CheckButton.outercolor.b"] = Id::CheckButton_outercolor_b;
      ids["CheckButton.outercolor.a"] = Id::CheckButton_outercolor_a;
      ids["DragButton"] = Id::DragButton;
      ids["DragButton.x"] = Id::Item_x;
      ids["DragButton.y"] = Id::Item_y;
      ids["DragButton.width"] = Id::Item_width;
      ids["DragButton.height"] = Id::Item_height;
      ids["DragButton.z"] = Id::Item_z;
      ids["DragButton.scale"] = Id::Item_scale;
      ids["DragButton.rotation"] = Id::Item_rotation;
      ids["DragButton.opacity"] = Id::Item_opacity;
      ids["DragButton.clip"] = Id::Item_clip;
      ids["DragButton.visible"] = Id::Item_visible;
      ids["DragButton.borderwidth"] = Id::Control_borderwidth;
      ids["DragButton.bordercolor"] = Id::Control_bordercolor;
      ids["DragButton.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["DragButton.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["DragButton.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["DragButton.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["DragButton.backgroundimage"] = Id::Control_backgroundimage;
      ids["DragButton.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["DragButton.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["DragButton.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["DragButton.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["DragButton.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["DragButton.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["DragButton.backgroundslice"] = Id::Control_backgroundslice;
      ids["DragButton.font"] = Id::Control_font;
      ids["DragButton.fontheight"] = Id::Control_fontheight;
      ids["DragButton.enabled"] = Id::Control_enabled;
      ids["DragButton.hovered"] = Id::Control_hovered;
      ids["DragButton.action"] = Id::Control_action;
      ids["DragButton.text"] = Id::Button_text;
      ids["DragButton.textcolor"] = Id::Button_textcolor;
      ids["DragButton.textcolor.r"] = Id::Button_textcolor_r;
      ids["DragButton.textcolor.g"] = Id::Button_textcolor_g;
      ids["DragButton.textcolor.b"] = Id::Button_textcolor_b;
      ids["DragButton.textcolor.a"] = Id::Button_textcolor_a;
      ids["DragButton.textalign"] = Id::Button_textalign;
      ids["DragButton.image"] = Id::Button_image;
      ids["DragButton.imagetint"] = Id::Button_imagetint;
      ids["DragButton.imagetint.r"] = Id::Button_imagetint_r;
      ids["DragButton.imagetint.g"] = Id::Button_imagetint_g;
      ids["DragButton.imagetint.b"] = Id::Button_imagetint_b;
      ids["DragButton.imagetint.a"] = Id::Button_imagetint_a;
      ids["DragButton.imagelayer"] = Id::Button_imagelayer;
      ids["DragButton.imagealign"] = Id::Button_imagealign;
      ids["DragButton.spacing"] = Id::Button_spacing;
      ids["DragButton.padding"] = Id::Button_padding;
      ids["DragButton.pressed"] = Id::Button_pressed;
      ids["DragButton.dx"] = Id::DragButton_dx;
      ids["DragButton.dy"] = Id::DragButton_dy;
      ids["Slider"] = Id::Slider;
      ids["Slider.x"] = Id::Item_x;
      ids["Slider.y"] = Id::Item_y;
      ids["Slider.width"] = Id::Item_width;
      ids["Slider.height"] = Id::Item_height;
      ids["Slider.z"] = Id::Item_z;
      ids["Slider.scale"] = Id::Item_scale;
      ids["Slider.rotation"] = Id::Item_rotation;
      ids["Slider.opacity"] = Id::Item_opacity;
      ids["Slider.clip"] = Id::Item_clip;
      ids["Slider.visible"] = Id::Item_visible;
      ids["Slider.borderwidth"] = Id::Control_borderwidth;
      ids["Slider.bordercolor"] = Id::Control_bordercolor;
      ids["Slider.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["Slider.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["Slider.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["Slider.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["Slider.backgroundimage"] = Id::Control_backgroundimage;
      ids["Slider.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["Slider.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["Slider.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["Slider.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["Slider.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["Slider.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["Slider.backgroundslice"] = Id::Control_backgroundslice;
      ids["Slider.font"] = Id::Control_font;
      ids["Slider.fontheight"] = Id::Control_fontheight;
      ids["Slider.enabled"] = Id::Control_enabled;
      ids["Slider.hovered"] = Id::Control_hovered;
      ids["Slider.action"] = Id::Control_action;
      ids["Slider.value"] = Id::Slider_value;
      ids["Slider.minvalue"] = Id::Slider_minvalue;
      ids["Slider.maxvalue"] = Id::Slider_maxvalue;
      ids["Slider.orientation"] = Id::Slider_orientation;
      ids["Slider.inset"] = Id::Slider_inset;
      ids["Slider.handleimage"] = Id::Slider_handleimage;
      ids["Slider.handletint"] = Id::Slider_handletint;
      ids["Slider.handletint.r"] = Id::Slider_handletint_r;
      ids["Slider.handletint.g"] = Id::Slider_handletint_g;
      ids["Slider.handletint.b"] = Id::Slider_handletint_b;
      ids["Slider.handletint.a"] = Id::Slider_handletint_a;
      ids["Slider.handlelayer"] = Id::Slider_handlelayer;
      ids["Slider.pressed"] = Id::Slider_pressed;
      ids["LineEdit"] = Id::LineEdit;
      ids["LineEdit.x"] = Id::Item_x;
      ids["LineEdit.y"] = Id::Item_y;
      ids["LineEdit.width"] = Id::Item_width;
      ids["LineEdit.height"] = Id::Item_height;
      ids["LineEdit.z"] = Id::Item_z;
      ids["LineEdit.scale"] = Id::Item_scale;
      ids["LineEdit.rotation"] = Id::Item_rotation;
      ids["LineEdit.opacity"] = Id::Item_opacity;
      ids["LineEdit.clip"] = Id::Item_clip;
      ids["LineEdit.visible"] = Id::Item_visible;
      ids["LineEdit.borderwidth"] = Id::Control_borderwidth;
      ids["LineEdit.bordercolor"] = Id::Control_bordercolor;
      ids["LineEdit.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["LineEdit.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["LineEdit.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["LineEdit.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["LineEdit.backgroundimage"] = Id::Control_backgroundimage;
      ids["LineEdit.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["LineEdit.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["LineEdit.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["LineEdit.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["LineEdit.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["LineEdit.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["LineEdit.backgroundslice"] = Id::Control_backgroundslice;
      ids["LineEdit.font"] = Id::Control_font;
      ids["LineEdit.fontheight"] = Id::Control_fontheight;
      ids["LineEdit.enabled"] = Id::Control_enabled;
      ids["LineEdit.hovered"] = Id::Control_hovered;
      ids["LineEdit.action"] = Id::Control_action;
      ids["LineEdit.text"] = Id::LineEdit_text;
      ids["LineEdit.textcolor"] = Id::LineEdit_textcolor;
      ids["LineEdit.textcolor.r"] = Id::LineEdit_textcolor_r;
      ids["LineEdit.textcolor.g"] = Id::LineEdit_textcolor_g;
      ids["LineEdit.textcolor.b"] = Id::LineEdit_textcolor_b;
      ids["LineEdit.textcolor.a"] = Id::LineEdit_textcolor_a;
      ids["LineEdit.padding"] = Id::LineEdit_padding;
      ids["LineEdit.selectioncolor"] = Id::LineEdit_selectioncolor;
      ids["LineEdit.selectioncolor.r"] = Id::LineEdit_selectioncolor_r;
      ids["LineEdit.selectioncolor.g"] = Id::LineEdit_selectioncolor_g;
      ids["LineEdit.selectioncolor.b"] = Id::LineEdit_selectioncolor_b;
      ids["LineEdit.selectioncolor.a"] = Id::LineEdit_selectioncolor_a;
      ids["LineEdit.selectiontextcolor"] = Id::LineEdit_selectiontextcolor;
      ids["LineEdit.selectiontextcolor.r"] = Id::LineEdit_selectiontextcolor_r;
      ids["LineEdit.selectiontextcolor.g"] = Id::LineEdit_selectiontextcolor_g;
      ids["LineEdit.selectiontextcolor.b"] = Id::LineEdit_selectiontextcolor_b;
      ids["LineEdit.selectiontextcolor.a"] = Id::LineEdit_selectiontextcolor_a;
      ids["LineEdit.focused"] = Id::LineEdit_focused;
      ids["LineEdit.filter"] = Id::LineEdit_filter;
      ids["SlideEdit"] = Id::SlideEdit;
      ids["SlideEdit.x"] = Id::Item_x;
      ids["SlideEdit.y"] = Id::Item_y;
      ids["SlideEdit.width"] = Id::Item_width;
      ids["SlideEdit.height"] = Id::Item_height;
      ids["SlideEdit.z"] = Id::Item_z;
      ids["SlideEdit.scale"] = Id::Item_scale;
      ids["SlideEdit.rotation"] = Id::Item_rotation;
      ids["SlideEdit.opacity"] = Id::Item_opacity;
      ids["SlideEdit.clip"] = Id::Item_clip;
      ids["SlideEdit.visible"] = Id::Item_visible;
      ids["SlideEdit.borderwidth"] = Id::Control_borderwidth;
      ids["SlideEdit.bordercolor"] = Id::Control_bordercolor;
      ids["SlideEdit.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["SlideEdit.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["SlideEdit.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["SlideEdit.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["SlideEdit.backgroundimage"] = Id::Control_backgroundimage;
      ids["SlideEdit.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["SlideEdit.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["SlideEdit.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["SlideEdit.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["SlideEdit.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["SlideEdit.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["SlideEdit.backgroundslice"] = Id::Control_backgroundslice;
      ids["SlideEdit.font"] = Id::Control_font;
      ids["SlideEdit.fontheight"] = Id::Control_fontheight;
      ids["SlideEdit.enabled"] = Id::Control_enabled;
      ids["SlideEdit.hovered"] = Id::Control_hovered;
      ids["SlideEdit.action"] = Id::Control_action;
      ids["SlideEdit.text"] = Id::LineEdit_text;
      ids["SlideEdit.textcolor"] = Id::LineEdit_textcolor;
      ids["SlideEdit.textcolor.r"] = Id::LineEdit_textcolor_r;
      ids["SlideEdit.textcolor.g"] = Id::LineEdit_textcolor_g;
      ids["SlideEdit.textcolor.b"] = Id::LineEdit_textcolor_b;
      ids["SlideEdit.textcolor.a"] = Id::LineEdit_textcolor_a;
      ids["SlideEdit.padding"] = Id::LineEdit_padding;
      ids["SlideEdit.selectioncolor"] = Id::LineEdit_selectioncolor;
      ids["SlideEdit.selectioncolor.r"] = Id::LineEdit_selectioncolor_r;
      ids["SlideEdit.selectioncolor.g"] = Id::LineEdit_selectioncolor_g;
      ids["SlideEdit.selectioncolor.b"] = Id::LineEdit_selectioncolor_b;
      ids["SlideEdit.selectioncolor.a"] = Id::LineEdit_selectioncolor_a;
      ids["SlideEdit.selectiontextcolor"] = Id::LineEdit_selectiontextcolor;
      ids["SlideEdit.selectiontextcolor.r"] = Id::LineEdit_selectiontextcolor_r;
      ids["SlideEdit.selectiontextcolor.g"] = Id::LineEdit_selectiontextcolor_g;
      ids["SlideEdit.selectiontextcolor.b"] = Id::LineEdit_selectiontextcolor_b;
      ids["SlideEdit.selectiontextcolor.a"] = Id::LineEdit_selectiontextcolor_a;
      ids["SlideEdit.focused"] = Id::LineEdit_focused;
      ids["SlideEdit.filter"] = Id::LineEdit_filter;
      ids["SlideEdit.value"] = Id::SlideEdit_value;
      ids["SlideEdit.precision"] = Id::SlideEdit_precision;
      ids["SlideEdit.format"] = Id::SlideEdit_format;
      ids["SlideEdit.minvalue"] = Id::SlideEdit_minvalue;
      ids["SlideEdit.maxvalue"] = Id::SlideEdit_maxvalue;
      ids["SlideEdit.handleimage"] = Id::SlideEdit_handleimage;
      ids["SlideEdit.handleimagetint"] = Id::SlideEdit_handleimagetint;
      ids["SlideEdit.handleimagetint.r"] = Id::SlideEdit_handleimagetint_r;
      ids["SlideEdit.handleimagetint.g"] = Id::SlideEdit_handleimagetint_g;
      ids["SlideEdit.handleimagetint.b"] = Id::SlideEdit_handleimagetint_b;
      ids["SlideEdit.handleimagetint.a"] = Id::SlideEdit_handleimagetint_a;
      ids["SlideEdit.handleimagelayer"] = Id::SlideEdit_handleimagelayer;
      ids["SlideEdit.sliderate"] = Id::SlideEdit_sliderate;
      ids["ListBox"] = Id::ListBox;
      ids["ListBox.x"] = Id::Item_x;
      ids["ListBox.y"] = Id::Item_y;
      ids["ListBox.width"] = Id::Item_width;
      ids["ListBox.height"] = Id::Item_height;
      ids["ListBox.z"] = Id::Item_z;
      ids["ListBox.scale"] = Id::Item_scale;
      ids["ListBox.rotation"] = Id::Item_rotation;
      ids["ListBox.opacity"] = Id::Item_opacity;
      ids["ListBox.clip"] = Id::Item_clip;
      ids["ListBox.visible"] = Id::Item_visible;
      ids["ListBox.borderwidth"] = Id::Control_borderwidth;
      ids["ListBox.bordercolor"] = Id::Control_bordercolor;
      ids["ListBox.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["ListBox.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["ListBox.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["ListBox.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["ListBox.backgroundimage"] = Id::Control_backgroundimage;
      ids["ListBox.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["ListBox.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["ListBox.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["ListBox.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["ListBox.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["ListBox.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["ListBox.backgroundslice"] = Id::Control_backgroundslice;
      ids["ListBox.font"] = Id::Control_font;
      ids["ListBox.fontheight"] = Id::Control_fontheight;
      ids["ListBox.enabled"] = Id::Control_enabled;
      ids["ListBox.hovered"] = Id::Control_hovered;
      ids["ListBox.action"] = Id::Control_action;
      ids["ListBox.text"] = Id::ListBox_text;
      ids["ListBox.textcolor"] = Id::ListBox_textcolor;
      ids["ListBox.textcolor.r"] = Id::ListBox_textcolor_r;
      ids["ListBox.textcolor.g"] = Id::ListBox_textcolor_g;
      ids["ListBox.textcolor.b"] = Id::ListBox_textcolor_b;
      ids["ListBox.textcolor.a"] = Id::ListBox_textcolor_a;
      ids["ListBox.handleimage"] = Id::ListBox_handleimage;
      ids["ListBox.handletint"] = Id::ListBox_handletint;
      ids["ListBox.handletint.r"] = Id::ListBox_handletint_r;
      ids["ListBox.handletint.g"] = Id::ListBox_handletint_g;
      ids["ListBox.handletint.b"] = Id::ListBox_handletint_b;
      ids["ListBox.handletint.a"] = Id::ListBox_handletint_a;
      ids["ListBox.handlelayer"] = Id::ListBox_handlelayer;
      ids["ListBox.handleslice"] = Id::ListBox_handleslice;
      ids["ListBox.handlesize"] = Id::ListBox_handlesize;
      ids["ListBox.handleinset"] = Id::ListBox_handleinset;
      ids["ListBox.padding"] = Id::ListBox_padding;
      ids["ListBox.selectioncolor"] = Id::ListBox_selectioncolor;
      ids["ListBox.selectioncolor.r"] = Id::ListBox_selectioncolor_r;
      ids["ListBox.selectioncolor.g"] = Id::ListBox_selectioncolor_g;
      ids["ListBox.selectioncolor.b"] = Id::ListBox_selectioncolor_b;
      ids["ListBox.selectioncolor.a"] = Id::ListBox_selectioncolor_a;
      ids["ListBox.selectiontextcolor"] = Id::ListBox_selectiontextcolor;
      ids["ListBox.selectiontextcolor.r"] = Id::ListBox_selectiontextcolor_r;
      ids["ListBox.selectiontextcolor.g"] = Id::ListBox_selectiontextcolor_g;
      ids["ListBox.selectiontextcolor.b"] = Id::ListBox_selectiontextcolor_b;
      ids["ListBox.selectiontextcolor.a"] = Id::ListBox_selectiontextcolor_a;
      ids["ListBox.focused"] = Id::ListBox_focused;
      ids["ComboBox"] = Id::ComboBox;
      ids["ComboBox.x"] = Id::Item_x;
      ids["ComboBox.y"] = Id::Item_y;
      ids["ComboBox.width"] = Id::Item_width;
      ids["ComboBox.height"] = Id::Item_height;
      ids["ComboBox.z"] = Id::Item_z;
      ids["ComboBox.scale"] = Id::Item_scale;
      ids["ComboBox.rotation"] = Id::Item_rotation;
      ids["ComboBox.opacity"] = Id::Item_opacity;
      ids["ComboBox.clip"] = Id::Item_clip;
      ids["ComboBox.visible"] = Id::Item_visible;
      ids["ComboBox.borderwidth"] = Id::Control_borderwidth;
      ids["ComboBox.bordercolor"] = Id::Control_bordercolor;
      ids["ComboBox.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["ComboBox.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["ComboBox.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["ComboBox.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["ComboBox.backgroundimage"] = Id::Control_backgroundimage;
      ids["ComboBox.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["ComboBox.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["ComboBox.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["ComboBox.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["ComboBox.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["ComboBox.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["ComboBox.backgroundslice"] = Id::Control_backgroundslice;
      ids["ComboBox.font"] = Id::Control_font;
      ids["ComboBox.fontheight"] = Id::Control_fontheight;
      ids["ComboBox.enabled"] = Id::Control_enabled;
      ids["ComboBox.hovered"] = Id::Control_hovered;
      ids["ComboBox.action"] = Id::Control_action;
      ids["ComboBox.text"] = Id::ComboBox_text;
      ids["ComboBox.textcolor"] = Id::ComboBox_textcolor;
      ids["ComboBox.textcolor.r"] = Id::ComboBox_textcolor_r;
      ids["ComboBox.textcolor.g"] = Id::ComboBox_textcolor_g;
      ids["ComboBox.textcolor.b"] = Id::ComboBox_textcolor_b;
      ids["ComboBox.textcolor.a"] = Id::ComboBox_textcolor_a;
      ids["ComboBox.dropimage"] = Id::ComboBox_dropimage;
      ids["ComboBox.dropimagetint"] = Id::ComboBox_dropimagetint;
      ids["ComboBox.dropimagetint.r"] = Id::ComboBox_dropimagetint_r;
      ids["ComboBox.dropimagetint.g"] = Id::ComboBox_dropimagetint_g;
      ids["ComboBox.dropimagetint.b"] = Id::ComboBox_dropimagetint_b;
      ids["ComboBox.dropimagetint.a"] = Id::ComboBox_dropimagetint_a;
      ids["ComboBox.dropimagelayer"] = Id::ComboBox_dropimagelayer;
      ids["ComboBox.listborderwidth"] = Id::ComboBox_listborderwidth;
      ids["ComboBox.listbordercolor"] = Id::ComboBox_listbordercolor;
      ids["ComboBox.listbordercolor.r"] = Id::ComboBox_listbordercolor_r;
      ids["ComboBox.listbordercolor.g"] = Id::ComboBox_listbordercolor_g;
      ids["ComboBox.listbordercolor.b"] = Id::ComboBox_listbordercolor_b;
      ids["ComboBox.listbordercolor.a"] = Id::ComboBox_listbordercolor_a;
      ids["ComboBox.listbackgroundcolor"] = Id::ComboBox_listbackgroundcolor;
      ids["ComboBox.listbackgroundcolor.r"] = Id::ComboBox_listbackgroundcolor_r;
      ids["ComboBox.listbackgroundcolor.g"] = Id::ComboBox_listbackgroundcolor_g;
      ids["ComboBox.listbackgroundcolor.b"] = Id::ComboBox_listbackgroundcolor_b;
      ids["ComboBox.listbackgroundcolor.a"] = Id::ComboBox_listbackgroundcolor_a;
      ids["ComboBox.handleimage"] = Id::ComboBox_handleimage;
      ids["ComboBox.handletint"] = Id::ComboBox_handletint;
      ids["ComboBox.handletint.r"] = Id::ComboBox_handletint_r;
      ids["ComboBox.handletint.g"] = Id::ComboBox_handletint_g;
      ids["ComboBox.handletint.b"] = Id::ComboBox_handletint_b;
      ids["ComboBox.handletint.a"] = Id::ComboBox_handletint_a;
      ids["ComboBox.handlelayer"] = Id::ComboBox_handlelayer;
      ids["ComboBox.handleslice"] = Id::ComboBox_handleslice;
      ids["ComboBox.handlesize"] = Id::ComboBox_handlesize;
      ids["ComboBox.handleinset"] = Id::ComboBox_handleinset;
      ids["ComboBox.padding"] = Id::ComboBox_padding;
      ids["ComboBox.selectioncolor"] = Id::ComboBox_selectioncolor;
      ids["ComboBox.selectioncolor.r"] = Id::ComboBox_selectioncolor_r;
      ids["ComboBox.selectioncolor.g"] = Id::ComboBox_selectioncolor_g;
      ids["ComboBox.selectioncolor.b"] = Id::ComboBox_selectioncolor_b;
      ids["ComboBox.selectioncolor.a"] = Id::ComboBox_selectioncolor_a;
      ids["ComboBox.selectiontextcolor"] = Id::ComboBox_selectiontextcolor;
      ids["ComboBox.selectiontextcolor.r"] = Id::ComboBox_selectiontextcolor_r;
      ids["ComboBox.selectiontextcolor.g"] = Id::ComboBox_selectiontextcolor_g;
      ids["ComboBox.selectiontextcolor.b"] = Id::ComboBox_selectiontextcolor_b;
      ids["ComboBox.selectiontextcolor.a"] = Id::ComboBox_selectiontextcolor_a;
      ids["ComboBox.pressed"] = Id::ComboBox_pressed;
      ids["ComboBox.dropped"] = Id::ComboBox_dropped;
      ids["GroupBox"] = Id::GroupBox;
      ids["GroupBox.x"] = Id::Item_x;
      ids["GroupBox.y"] = Id::Item_y;
      ids["GroupBox.width"] = Id::Item_width;
      ids["GroupBox.height"] = Id::Item_height;
      ids["GroupBox.z"] = Id::Item_z;
      ids["GroupBox.scale"] = Id::Item_scale;
      ids["GroupBox.rotation"] = Id::Item_rotation;
      ids["GroupBox.opacity"] = Id::Item_opacity;
      ids["GroupBox.clip"] = Id::Item_clip;
      ids["GroupBox.visible"] = Id::Item_visible;
      ids["GroupBox.borderwidth"] = Id::Control_borderwidth;
      ids["GroupBox.bordercolor"] = Id::Control_bordercolor;
      ids["GroupBox.bordercolor.r"] = Id::Control_bordercolor_r;
      ids["GroupBox.bordercolor.g"] = Id::Control_bordercolor_g;
      ids["GroupBox.bordercolor.b"] = Id::Control_bordercolor_b;
      ids["GroupBox.bordercolor.a"] = Id::Control_bordercolor_a;
      ids["GroupBox.backgroundimage"] = Id::Control_backgroundimage;
      ids["GroupBox.backgroundcolor"] = Id::Control_backgroundcolor;
      ids["GroupBox.backgroundcolor.r"] = Id::Control_backgroundcolor_r;
      ids["GroupBox.backgroundcolor.g"] = Id::Control_backgroundcolor_g;
      ids["GroupBox.backgroundcolor.b"] = Id::Control_backgroundcolor_b;
      ids["GroupBox.backgroundcolor.a"] = Id::Control_backgroundcolor_a;
      ids["GroupBox.backgroundlayer"] = Id::Control_backgroundlayer;
      ids["GroupBox.backgroundslice"] = Id::Control_backgroundslice;
      ids["GroupBox.font"] = Id::Control_font;
      ids["GroupBox.fontheight"] = Id::Control_fontheight;
      ids["GroupBox.enabled"] = Id::Control_enabled;
      ids["GroupBox.hovered"] = Id::Control_hovered;
      ids["GroupBox.action"] = Id::Control_action;
      ids["GroupBox.text"] = Id::Button_text;
      ids["GroupBox.textcolor"] = Id::Button_textcolor;
      ids["GroupBox.textcolor.r"] = Id::Button_textcolor_r;
      ids["GroupBox.textcolor.g"] = Id::Button_textcolor_g;
      ids["GroupBox.textcolor.b"] = Id::Button_textcolor_b;
      ids["GroupBox.textcolor.a"] = Id::Button_textcolor_a;
      ids["GroupBox.textalign"] = Id::Button_textalign;
      ids["GroupBox.image"] = Id::Button_image;
      ids["GroupBox.imagetint"] = Id::Button_imagetint;
      ids["GroupBox.imagetint.r"] = Id::Button_imagetint_r;
      ids["GroupBox.imagetint.g"] = Id::Button_imagetint_g;
      ids["GroupBox.imagetint.b"] = Id::Button_imagetint_b;
      ids["GroupBox.imagetint.a"] = Id::Button_imagetint_a;
      ids["GroupBox.imagelayer"] = Id::Button_imagelayer;
      ids["GroupBox.imagealign"] = Id::Button_imagealign;
      ids["GroupBox.spacing"] = Id::Button_spacing;
      ids["GroupBox.padding"] = Id::Button_padding;
      ids["GroupBox.pressed"] = Id::Button_pressed;
      ids["GroupBox.checked"] = Id::CheckButton_checked;
      ids["GroupBox.innercolor"] = Id::CheckButton_innercolor;
      ids["GroupBox.innercolor.r"] = Id::CheckButton_innercolor_r;
      ids["GroupBox.innercolor.g"] = Id::CheckButton_innercolor_g;
      ids["GroupBox.innercolor.b"] = Id::CheckButton_innercolor_b;
      ids["GroupBox.innercolor.a"] = Id::CheckButton_innercolor_a;
      ids["GroupBox.outercolor"] = Id::CheckButton_outercolor;
      ids["GroupBox.outercolor.r"] = Id::CheckButton_outercolor_r;
      ids["GroupBox.outercolor.g"] = Id::CheckButton_outercolor_g;
      ids["GroupBox.outercolor.b"] = Id::CheckButton_outercolor_b;
      ids["GroupBox.outercolor.a"] = Id::CheckButton_outercolor_a;
    }

    ///////////////////////// init_constants_map ////////////////////////////
    [[gnu::unused]] void init_constants_map(std::unordered_map<std::string, int> &constants)
    {
      constants["Text.Left"] = Text::Left;
      constants["Text.Center"] = Text::Center;
      constants["Text.Right"] = Text::Right;
      constants["Text.Justified"] = Text::Justified;
      constants["Text.Top"] = Text::Top;
      constants["Text.Middle"] = Text::Middle;
      constants["Text.Bottom"] = Text::Bottom;
      constants["Text.BaseLine"] = Text::BaseLine;

      constants["Image.Stretch"] = Image::Stretch;
      constants["Image.Fit"] = Image::Fit;
      constants["Image.Crop"] = Image::Crop;
      constants["Image.Pad"] = Image::Pad;
      constants["Image.Left"] = Image::Left;
      constants["Image.Center"] = Image::Center;
      constants["Image.Right"] = Image::Right;
      constants["Image.Top"] = Image::Top;
      constants["Image.Middle"] = Image::Middle;
      constants["Image.Bottom"] = Image::Bottom;

      constants["ScrollBar.Horizontal"] = ScrollBar::Horizontal;
      constants["ScrollBar.Vertical"] = ScrollBar::Vertical;

      constants["TextArea.Left"] = TextArea::Left;
      constants["TextArea.Center"] = TextArea::Center;
      constants["TextArea.Right"] = TextArea::Right;
      constants["TextArea.Justified"] = TextArea::Justified;
      constants["TextArea.Top"] = TextArea::Top;
      constants["TextArea.Middle"] = TextArea::Middle;
      constants["TextArea.Bottom"] = TextArea::Bottom;

      constants["Label.Left"] = Label::Left;
      constants["Label.Center"] = Label::Center;
      constants["Label.Right"] = Label::Right;
      constants["Label.Justified"] = Label::Justified;
      constants["Label.Top"] = Label::Top;
      constants["Label.Middle"] = Label::Middle;
      constants["Label.Bottom"] = Label::Bottom;
      constants["Label.BaseLine"] = Label::BaseLine;

      constants["Field.Left"] = Field::Left;
      constants["Field.Center"] = Field::Center;
      constants["Field.Right"] = Field::Right;
      constants["Field.Top"] = Field::Top;
      constants["Field.Middle"] = Field::Middle;
      constants["Field.Bottom"] = Field::Bottom;
      constants["Field.BaseLine"] = Field::BaseLine;

      constants["Button.Left"] = Button::Left;
      constants["Button.Center"] = Button::Center;
      constants["Button.Right"] = Button::Right;
      constants["Button.Before"] = Button::Before;
      constants["Button.Fill"] = Button::Fill;
      constants["Button.After"] = Button::After;
      constants["Button.Above"] = Button::Above;
      constants["Button.Below"] = Button::Below;

      constants["CheckButton.Left"] = CheckButton::Left;
      constants["CheckButton.Center"] = CheckButton::Center;
      constants["CheckButton.Right"] = CheckButton::Right;
      constants["CheckButton.Before"] = CheckButton::Before;
      constants["CheckButton.Fill"] = CheckButton::Fill;
      constants["CheckButton.After"] = CheckButton::After;
      constants["CheckButton.Above"] = CheckButton::Above;
      constants["CheckButton.Below"] = CheckButton::Below;
      constants["CheckButton.UnChecked"] = CheckButton::UnChecked;
      constants["CheckButton.Indeterminate"] = CheckButton::Indeterminate;
      constants["CheckButton.Checked"] = CheckButton::Checked;

      constants["Slider.Horizontal"] = Slider::Horizontal;
      constants["Slider.Vertical"] = Slider::Vertical;

      constants["LineEdit.Readonly"] = LineEdit::Readonly;
      constants["LineEdit.Integer"] = LineEdit::Integer;
      constants["LineEdit.Decimal"] = LineEdit::Decimal;
      constants["LineEdit.Text"] = LineEdit::Text;

      constants["GroupBox.Left"] = GroupBox::Left;
      constants["GroupBox.Center"] = GroupBox::Center;
      constants["GroupBox.Right"] = GroupBox::Right;
      constants["GroupBox.Before"] = GroupBox::Before;
      constants["GroupBox.Fill"] = GroupBox::Fill;
      constants["GroupBox.After"] = GroupBox::After;
      constants["GroupBox.Above"] = GroupBox::Above;
      constants["GroupBox.Below"] = GroupBox::Below;
      constants["GroupBox.UnChecked"] = GroupBox::UnChecked;
      constants["GroupBox.Indeterminate"] = GroupBox::Indeterminate;
      constants["GroupBox.Checked"] = GroupBox::Checked;

    }

#endif
  }
}
