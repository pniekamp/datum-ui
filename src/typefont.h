//
// datum-ui - typefont
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/renderer.h"

//|---------------------- TypeFace ------------------------------------------
//|--------------------------------------------------------------------------

class TypeFace
{
  public:
    friend TypeFace const *ResourceManager::create<TypeFace>(Asset const *asset);

    bool ready() const { return (state == State::Ready); }

    const void *fontdata;

  public:

    enum class State
    {
      Empty,
      Loading,
      Ready,
    };

    Asset const *asset;

    std::atomic<State> state;

    size_t datasize;

  protected:
    TypeFace() = default;
};


//|---------------------- TypeFont ------------------------------------------
//|--------------------------------------------------------------------------

class TypeFont
{
  public:
    friend TypeFont const *ResourceManager::create<TypeFont>(TypeFace const *typeface, float pixelheight);

    bool ready() const { return (state == State::Ready); }

    int ascent;
    int descent;
    int leading;
    float pixelscale;
    float pixelheight;

    float width(const char *str, size_t n = size_t(-1)) const;
    float width(uint32_t codepoint, uint32_t nextcodepoint) const;
    float height() const;
    float lineheight() const;

    TypeFace const *typeface;

  public:

    enum class State
    {
      Empty,
      Loading,
      Ready,
    };

    std::atomic<State> state;

    struct Glyph
    {
      uint32_t codepoint;
      lml::Vec4 texcoords;
      lml::Vec2 alignment;
      lml::Vec2 dimension;
      Texture const *sheet;
    };

    int glyphcount;
    Glyph glyphs[512];

    Texture const *sheets[4];

    int activesheet;
    int activesheetused;

    Glyph const *find(uint32_t codepoint) const;

    ResourceManager::TransferLump const *transferlump;

    alignas(16) uint8_t fontinfo[152];

    mutable leap::threadlib::SpinLock mutex;

  protected:
    TypeFont() = default;
};

struct TextAlign
{
  enum
  {
    Top = 0x1,
    Middle = 0x02,
    Bottom = 0x4,
    BaseLine = 0x8,

    Left = 0x10,
    Center = 0x20,
    Right = 0x40,
    Justified = 0x80,
  };
};

size_t pick_text(TypeFont const *font, const char *str, size_t n, float extraspace, float x);

void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, lml::Vec2 const &position, lml::Vec2 const &xbasis, lml::Vec2 const &ybasis, TypeFont const *font, const char *str, size_t n, float extraspace, lml::Color4 const &tint);

void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, lml::Vec2 const &position, TypeFont const *font, const char *str, lml::Color4 const &tint = { 1.0f, 1.0f, 1.0f, 1.0f });
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, lml::Vec2 const &position, float rotation, TypeFont const *font, const char *str, lml::Color4 const &tint = { 1.0f, 1.0f, 1.0f, 1.0f });
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, lml::Vec2 const &position, lml::Vec2 const &xbasis, lml::Vec2 const &ybasis, TypeFont const *font, const char *str, lml::Color4 const &tint = { 1.0f, 1.0f, 1.0f, 1.0f });

void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, lml::Vec2 const &position, lml::Rect2 const &rect, long alignment, TypeFont const *font, const char *str, lml::Color4 const &tint = { 1.0f, 1.0f, 1.0f, 1.0f });
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, lml::Vec2 const &position, lml::Rect2 const &rect, long alignment, float rotation, TypeFont const *font, const char *str, lml::Color4 const &tint = { 1.0f, 1.0f, 1.0f, 1.0f });
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, lml::Vec2 const &position, lml::Vec2 const &xbasis, lml::Vec2 const &ybasis, lml::Rect2 const &rect, long alignment, TypeFont const *font, const char *str, lml::Color4 const &tint = { 1.0f, 1.0f, 1.0f, 1.0f });

