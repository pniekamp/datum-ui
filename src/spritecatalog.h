//
// datum-ui - sprite catalog
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/memory.h"
#include "datum/renderer.h"

using SpriteAtlas = Texture;
using SpriteImage = Sprite;

namespace Ui
{
  using Sprite = shared_resource<SpriteImage>;

  //|---------------------- SpriteCatalog -------------------------------------
  //|--------------------------------------------------------------------------

  class SpriteCatalog
  {
    public:

      using allocator_type = StackAllocator<>;

      SpriteCatalog(ResourceManager &resources, allocator_type const &allocator);

      SpriteCatalog(SpriteCatalog const &) = delete;

    public:

      void add(leap::string_view name, Asset const *asset);

      size_t count() const;
      leap::string_view entry(size_t i) const;

      SpriteAtlas const *find(leap::string_view name) const;

      Sprite find(leap::string_view name, lml::Rect2 region, lml::Vec2 align = { 0.0f, 0.0f }) const;

      void request(DatumPlatform::PlatformInterface &platform, SpriteAtlas const *spriteatlas);
      void request(DatumPlatform::PlatformInterface &platform, Sprite const &sprite);

      void sweep_resources();

    private:

      allocator_type m_allocator;

    private:

      ResourceManager *m_resources;

    private:

      struct Entry
      {
        size_t namelen;
        const char *name;

        Texture const *spriteatlas;

        size_t instancecount;
        SpriteImage const *instances[256];
        std::atomic<int> *refcounts[256];
      };

      std::vector<Entry*, StackAllocator<Entry*>> m_entries;

      mutable leap::threadlib::SpinLock m_mutex;
  };
}

// Request Utility
inline void request(DatumPlatform::PlatformInterface &platform, Ui::SpriteCatalog &catalog, Ui::Sprite &sprite, int *ready, int *total)
{
  if (sprite)
  {
    *total += 1;

    catalog.request(platform, sprite);

    if (sprite->ready())
      *ready += 1;
  }
}
