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

using SpriteSheet = Sprite;

namespace Ui
{
  using Sprite = shared_resource<SpriteSheet>;

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

      Sprite find(leap::string_view name, lml::Rect2 region = { { 0.0f, 0.0f }, { 1.0f, 1.0f } }, lml::Vec2 align = { 0.0f, 0.0f }) const;

      bool request_resources(DatumPlatform::PlatformInterface &platform);

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

        Texture const *spritesheet;

        size_t instancecount;
        SpriteSheet const *instances[256];
        std::atomic<int> *refcounts[256];
      };

      std::vector<Entry*, StackAllocator<Entry*>> m_entries;

      mutable leap::threadlib::SpinLock m_mutex;
  };
}
