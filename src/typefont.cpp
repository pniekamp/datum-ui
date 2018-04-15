//
// datum-ui - typefont
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "typefont.h"
#include <algorithm>
#include "datum/debug.h"

using namespace std;
using namespace lml;
using leap::extentof;

namespace
{
  //
  // STB TrueType
  // authored from 2009-2016 by Sean Barrett / RAD Game Tools
  //

# define STB_TRUETYPE_IMPLEMENTATION
# include "stb_truetype.h"

  static_assert(sizeof(stbtt_fontinfo) <= sizeof(TypeFont::fontinfo), "FontInfo Size Mismatch");
}

namespace
{
  //
  // UTF-8 decode
  // Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
  // See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
  //

  static const uint8_t utf8d[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
    8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
    0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
    0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
    0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
    1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
    1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
    1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
  };

  uint32_t utf8_decode(uint32_t *state, uint32_t *codepoint, char ch)
  {
    uint32_t byte = uint8_t(ch);
    uint32_t type = utf8d[byte];

    *codepoint = (*state != 0) ? (byte & 0x3fu) | (*codepoint << 6) : (0xff >> type) & (byte);

    *state = utf8d[256 + *state*16 + type];

    return *state;
  }
}

namespace
{
  int font_sheetpad()
  {
    return 2;
  }

  int font_sheetwidth(TypeFont const *font)
  {
    return 1024;
  }

  int font_sheetheight(TypeFont const *font)
  {
    return (int)((font->ascent + 2*font->descent) * font->pixelscale) + font_sheetpad() + font_sheetpad();
  }
}


//|---------------------- TypeFace -----------------------------------------
//|--------------------------------------------------------------------------

///////////////////////// ResourceManager::create ///////////////////////////
template<>
TypeFace const *ResourceManager::create<TypeFace>(Asset const *asset)
{
  if (!asset)
    return nullptr;

  auto slot = acquire_slot(sizeof(TypeFace));

  if (!slot)
    return nullptr;

  auto typeface = new(slot) TypeFace;

  typeface->datasize = 0;
  typeface->fontdata = nullptr;
  typeface->asset = asset;
  typeface->state = TypeFace::State::Empty;

  return typeface;
}


///////////////////////// ResourceManager::request //////////////////////////
template<>
void ResourceManager::request<TypeFace>(DatumPlatform::PlatformInterface &platform, TypeFace const *typeface)
{
  assert(typeface);

  auto slot = const_cast<TypeFace*>(typeface);

  TypeFace::State empty = TypeFace::State::Empty;

  if (slot->state.compare_exchange_strong(empty, TypeFace::State::Loading))
  {
    if (auto asset = slot->asset)
    {
      assert(assets()->barriercount != 0);

      if (auto bits = m_assets->request(platform, asset))
      {
        auto fontdata = system_allocator_type<uint8_t>{}.allocate(asset->datasize);

        memcpy(fontdata, bits, asset->datasize);

        slot->fontdata = fontdata;
        slot->datasize = asset->datasize;
      }
    }

    slot->state = (slot->fontdata) ? TypeFace::State::Ready : TypeFace::State::Empty;
  }
}


///////////////////////// ResourceManager::release //////////////////////////
template<>
void ResourceManager::release<TypeFace>(TypeFace const *typeface)
{
  defer_destroy(typeface);
}


///////////////////////// ResourceManager::destroy //////////////////////////
template<>
void ResourceManager::destroy<TypeFace>(TypeFace const *typeface)
{
  if (typeface)
  {
    if (typeface->fontdata)
      system_allocator_type<uint8_t>{}.deallocate((uint8_t*)(typeface->fontdata), typeface->datasize);

    typeface->~TypeFace();

    release_slot(const_cast<TypeFace*>(typeface), sizeof(TypeFace));
  }
}


//|---------------------- TypeFont ------------------------------------------
//|--------------------------------------------------------------------------

///////////////////////// ResourceManager::update ///////////////////////////
template<>
void ResourceManager::update<TypeFont>(TypeFont const *font, TypeFace const *typeface)
{
  assert(font);
  assert(typeface->ready());
  assert(font->transferlump == nullptr);

  auto slot = const_cast<TypeFont*>(font);

  slot->typeface = typeface;

  stbtt_InitFont((stbtt_fontinfo*)slot->fontinfo, (const unsigned char *)slot->typeface->fontdata, 0);

  slot->pixelscale = stbtt_ScaleForPixelHeight((stbtt_fontinfo*)slot->fontinfo, slot->pixelheight);

  stbtt_GetFontVMetrics((stbtt_fontinfo*)slot->fontinfo, &slot->ascent, &slot->descent, &slot->leading);

  slot->descent = -slot->descent;

  int maxheight = font_sheetheight(slot);

  slot->transferlump = acquire_lump(4 * maxheight * maxheight * sizeof(uint32_t));
}


///////////////////////// ResourceManager::update ///////////////////////////
template<>
void ResourceManager::update<TypeFont>(TypeFont const *font, uint32_t codepoint)
{
  assert(font);

  if (codepoint <= 32)
    return;

  auto slot = const_cast<TypeFont*>(font);

  int width, height, offsetx, offsety;
  auto bitmap = stbtt_GetCodepointBitmap((stbtt_fontinfo const *)font->fontinfo, font->pixelscale, font->pixelscale, codepoint, &width, &height, &offsetx, &offsety);

  if (bitmap && slot->transferlump)
  {
    int sheetpad = font_sheetpad();
    int sheetwidth = font_sheetwidth(font);
    int sheetheight = font_sheetheight(font);

    int paddedwidth = width + sheetpad + sheetpad;
    int paddedheight = height + sheetpad + sheetpad;

    assert(paddedheight <= sheetheight);
    assert(paddedwidth * paddedheight * sizeof(uint32_t) < slot->transferlump->transferbuffer.size);

    if (slot->activesheetused + paddedwidth > sheetwidth)
    {
      slot->activesheet = (slot->activesheet + 1) % extentof(slot->sheets);

      auto j = remove_if(slot->glyphs, slot->glyphs + slot->glyphcount, [&](auto &glyph) { return glyph.sheet == slot->sheets[slot->activesheet]; });

      slot->glyphcount = j - slot->glyphs;

      release(slot->sheets[slot->activesheet]);

      slot->sheets[slot->activesheet] = create<Texture>(sheetwidth, sheetheight, 1, 1, Texture::Format::RGBA);

      slot->activesheetused = 0;
    }

    if (slot->sheets[slot->activesheet])
    {
      auto *data = slot->transferlump->memory<uint32_t>();

      memset(data, 0, paddedwidth * paddedheight * sizeof(uint32_t));

      for(int j = 0; j < height; ++j)
      {
        uint8_t *src = bitmap + j * width;
        uint32_t *dst = data + (j + sheetpad) * paddedwidth + sheetpad;

        for(int i = 0; i < width; ++i)
        {
          dst[i] = (src[i] << 0) | (src[i] << 8) | (src[i] << 16) | (src[i] << 24);
        }
      }

      float sx = 1.0f / sheetwidth;
      float sy = 1.0f / sheetheight;

      auto glyph = lower_bound(slot->glyphs, slot->glyphs + slot->glyphcount, codepoint, [](auto &glyph, uint32_t codepoint) { return glyph.codepoint < codepoint; });

      if (glyph == slot->glyphs + slot->glyphcount || glyph->codepoint != codepoint)
      {
        if (slot->glyphcount < (int)extentof(slot->glyphs))
        {
          move_backward(glyph, slot->glyphs + slot->glyphcount, slot->glyphs + slot->glyphcount + 1);

          slot->glyphcount += 1;
        }
        else if (glyph == slot->glyphs + slot->glyphcount)
        {
          glyph -= 1;
        }
      }

      glyph->codepoint = codepoint;
      glyph->texcoords = Vec4(sx * (slot->activesheetused + sheetpad - 1), sy * (sheetpad - 1), sx * (width + 2), sy * (height + 2));
      glyph->alignment = Vec2(1 - offsetx, 1 - offsety);
      glyph->dimension = Vec2(width + 2, height + 2);
      glyph->sheet = slot->sheets[slot->activesheet];

      update(glyph->sheet, slot->transferlump, slot->activesheetused, 0, paddedwidth, paddedheight, 0, 0);

      slot->activesheetused += paddedwidth;
    }
  }
}


///////////////////////// ResourceManager::create ///////////////////////////
template<>
TypeFont const *ResourceManager::create<TypeFont>(TypeFace const *typeface, float pixelheight)
{
  assert(pixelheight > 0.0f);
  assert(pixelheight < 256.0f);

  if (!typeface)
    return nullptr;

  auto slot = acquire_slot(sizeof(TypeFont));

  if (!slot)
    return nullptr;

  auto font = new(slot) TypeFont;

  font->ascent = 0;
  font->descent = 0;
  font->leading = 0;
  font->pixelscale = 0;
  font->typeface = typeface;
  font->pixelheight = pixelheight;

  font->glyphcount = 0;
  font->activesheet = -1;
  font->activesheetused = 9999;
  font->transferlump = nullptr;
  font->state = TypeFont::State::Empty;

  memset(font->sheets, 0, sizeof(font->sheets));

  if (typeface->ready())
  {
    update(font, typeface);

    font->state = TypeFont::State::Ready;
  }

  return font;
}


///////////////////////// ResourceManager::request //////////////////////////
template<>
void ResourceManager::request<TypeFont>(DatumPlatform::PlatformInterface &platform, TypeFont const *font)
{
  assert(font);

  auto slot = const_cast<TypeFont*>(font);

  TypeFont::State empty = TypeFont::State::Empty;

  if (slot->state.compare_exchange_strong(empty, TypeFont::State::Loading))
  {
    request(platform, slot->typeface);

    if (slot->typeface->ready())
    {
      update(font, slot->typeface);
    }

    slot->state = (slot->typeface->ready()) ? TypeFont::State::Ready : TypeFont::State::Empty;
  }
}


///////////////////////// ResourceManager::release //////////////////////////
template<>
void ResourceManager::release<TypeFont>(TypeFont const *font)
{
  defer_destroy(font);
}


///////////////////////// ResourceManager::destroy //////////////////////////
template<>
void ResourceManager::destroy<TypeFont>(TypeFont const *font)
{
  if (font)
  {
    for(auto &sheet : font->sheets)
      destroy(sheet);

    if (font->transferlump)
      release_lump(font->transferlump);

    font->~TypeFont();

    release_slot(const_cast<TypeFont*>(font), sizeof(TypeFont));
  }
}


///////////////////////// TypeFont::width ///////////////////////////////////
float TypeFont::width(const char *str, size_t n) const
{
  float sum = 0;

  uint32_t codepoint;
  uint32_t lastcodepoint = 0;
  uint32_t codepointstate = 0;

  for(const char *ch = str; *ch && n; ++ch, --n)
  {
    if (utf8_decode(&codepointstate, &codepoint, *ch) != 0)
      continue;

    sum += width(lastcodepoint, codepoint);

    lastcodepoint = codepoint;
  }

  sum += width(lastcodepoint, 0);

  return sum;
}


///////////////////////// TypeFont::width ///////////////////////////////////
float TypeFont::width(uint32_t codepoint, uint32_t nextcodepoint) const
{
  int advance = 0, lsb = 0, kerning = 0;

  if (codepoint != 0)
  {
    stbtt_GetCodepointHMetrics((stbtt_fontinfo const *)fontinfo, codepoint, &advance, &lsb);

    if (nextcodepoint != 0)
    {
      kerning = stbtt_GetCodepointKernAdvance((stbtt_fontinfo const *)fontinfo, codepoint, nextcodepoint);
    }
  }

  return round((advance + kerning) * pixelscale);
}


///////////////////////// TypeFont::height //////////////////////////////////
float TypeFont::height() const
{
  return (ascent + descent) * pixelscale;
}


///////////////////////// TypeFont::lineheight //////////////////////////////
float TypeFont::lineheight() const
{
  return (ascent + descent + leading) * pixelscale;
}


///////////////////////// TypeFont::find  ///////////////////////////////////
TypeFont::Glyph const *TypeFont::find(uint32_t codepoint) const
{
  auto glyph = lower_bound(glyphs, glyphs + glyphcount, codepoint, [](auto &glyph, uint32_t codepoint) { return glyph.codepoint < codepoint; });

  if (glyph != glyphs + glyphcount && glyph->codepoint == codepoint)
  {
    return glyph;
  }

  return nullptr;
}


///////////////////////// pick_text /////////////////////////////////////////
size_t pick_text(TypeFont const *font, const char *str, size_t n, float extraspace, float x)
{
  assert(font && font->ready());

  float cursor = 0.0f;

  size_t index = 0;
  size_t lastindex = 0;

  uint32_t codepoint = 0;
  uint32_t lastcodepoint = 0;
  uint32_t codepointstate = 0;

  for(const char *ch = str; *ch && n; ++ch, --n)
  {
    if (utf8_decode(&codepointstate, &codepoint, *ch) != 0)
      continue;

    cursor += font->width(lastcodepoint, codepoint);

    if (x < cursor)
      return lastindex;

    if (codepoint == 32)
      cursor += extraspace;

    lastindex = index;
    lastcodepoint = codepoint;

    index = (ch + 1) - str;
  }

  cursor += font->width(codepoint, 0);

  if (x < cursor)
    return lastindex;

  return index;
}


///////////////////////// draw_text /////////////////////////////////////////
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, Vec2 const &position, Vec2 const &xbasis, Vec2 const &ybasis, TypeFont const *font, const char *str, size_t n, float extraspace, Color4 const &tint)
{
  assert(font && font->ready());

  leap::threadlib::SyncLock lock(font->mutex);

  auto cursor = position;

  uint32_t codepoint;
  uint32_t lastcodepoint = 0;
  uint32_t codepointstate = 0;

  for(const char *ch = str; *ch && n; ++ch, --n)
  {
    if (utf8_decode(&codepointstate, &codepoint, *ch) != 0)
      continue;

    cursor += (font->width(lastcodepoint, codepoint) + ((codepoint == 32) ? extraspace : 0.0f)) * xbasis;

    auto glyph = font->find(codepoint);

    if (!glyph)
    {
      buildstate.resources->update(font, codepoint);

      glyph = font->find(codepoint);
    }

    if (glyph)
    {
      if (buildstate.texture != glyph->sheet->texture || buildstate.color != tint)
      {
        spritelist.push_material(buildstate, glyph->sheet->texture, tint);
      }

      spritelist.push_model(buildstate, cursor - glyph->alignment.x * xbasis - glyph->alignment.y * ybasis, glyph->dimension.x * xbasis, glyph->dimension.y * ybasis, glyph->texcoords, 0, 0);
    }

    lastcodepoint = codepoint;
  }
}


///////////////////////// draw_text /////////////////////////////////////////
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, Vec2 const &position, Vec2 const &xbasis, Vec2 const &ybasis, TypeFont const *font, const char *str, Color4 const &tint)
{
  draw_text(spritelist, buildstate, position, xbasis, ybasis, font, str, size_t(-1), 0.0f, tint);
}


///////////////////////// draw_text /////////////////////////////////////////
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, Vec2 const &position, TypeFont const *font, const char *str, Color4 const &tint)
{
  auto xbasis = Vec2(1, 0);
  auto ybasis = Vec2(0, 1);

  draw_text(spritelist, buildstate, position, xbasis, ybasis, font, str, tint);
}


///////////////////////// draw_text /////////////////////////////////////////
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, Vec2 const &position, float rotation, TypeFont const *font, const char *str, Color4 const &tint)
{
  auto xbasis = rotate(Vec2(1, 0), rotation);
  auto ybasis = rotate(Vec2(0, 1), rotation);

  draw_text(spritelist, buildstate, position, xbasis, ybasis, font, str, tint);
}


///////////////////////// draw_text /////////////////////////////////////////
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, Vec2 const &position, Vec2 const &xbasis, Vec2 const &ybasis, Rect2 const &rect, long alignment, TypeFont const *font, const char *str, Color4 const &tint)
{
  assert(font && font->ready());

  auto cursor = position;

  if (alignment & TextAlign::Left)
  {
    cursor += min(rect.min.x, rect.max.x) * xbasis;
  }

  else if (alignment & TextAlign::Center)
  {
    cursor += (0.5f*(rect.min.x + rect.max.x - font->width(str))) * xbasis;
  }

  else if (alignment & TextAlign::Right)
  {
    cursor += (max(rect.min.x, rect.max.x) - font->width(str)) * xbasis;
  }

  if (alignment & TextAlign::Top)
  {
    cursor += (min(rect.min.y, rect.max.y) + font->ascent*font->pixelscale) * ybasis;
  }

  else if (alignment & TextAlign::Middle)
  {
    cursor += (0.5f*(rect.min.y + rect.max.y - font->height()) + font->ascent*font->pixelscale) * ybasis;
  }

  else if (alignment & TextAlign::BaseLine)
  {
    cursor += max(rect.min.y, rect.max.y) * ybasis;
  }

  else if (alignment & TextAlign::Bottom)
  {
    cursor += (max(rect.min.y, rect.max.y) - font->descent*font->pixelscale) * ybasis;
  }

  float extraspace = 0.0f;

  if (alignment & TextAlign::Justified)
  {
    int spaces = 0;
    for(const char *ch = str; *ch; ++ch)
    {
      if (*ch == 32)
        ++spaces;
    }

    extraspace = (abs(rect.width()) - font->width(str)) / spaces;

    cursor += min(rect.min.x, rect.max.x) * xbasis;
  }

  draw_text(spritelist, buildstate, cursor, xbasis, ybasis, font, str, size_t(-1), extraspace, tint);
}


///////////////////////// draw_text /////////////////////////////////////////
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, Vec2 const &position, Rect2 const &rect, long alignment, TypeFont const *font, const char *str, Color4 const &tint)
{
  auto xbasis = Vec2(1, 0);
  auto ybasis = Vec2(0, 1);

  draw_text(spritelist, buildstate, position, xbasis, ybasis, rect, alignment, font, str, tint);
}


///////////////////////// draw_text /////////////////////////////////////////
void draw_text(SpriteList &spritelist, SpriteList::BuildState &buildstate, Vec2 const &position, Rect2 const &rect, long alignment, float rotation, TypeFont const *font, const char *str, Color4 const &tint)
{
  auto xbasis = rotate(Vec2(1, 0), rotation);
  auto ybasis = rotate(Vec2(0, 1), rotation);

  draw_text(spritelist, buildstate, position, xbasis, ybasis, rect, alignment, font, str, tint);
}
