//
// datum-ui - font catalog
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include "datum.h"
#include "datum/memory.h"
#include "typefont.h"

namespace Ui
{
  using Font = shared_resource<TypeFont>;

  //|---------------------- FontCatalog ---------------------------------------
  //|--------------------------------------------------------------------------

  class FontCatalog
  {
    public:

      using allocator_type = StackAllocator<>;

      FontCatalog(ResourceManager &resources, allocator_type const &allocator);

      FontCatalog(FontCatalog const &) = delete;

    public:

      void add(leap::string_view name, Asset const *asset);

      TypeFace const *find(leap::string_view name) const;

      Font find(leap::string_view name, float pixelheight) const;
      Font find(TypeFace const *typeface, float pixelheight) const;

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

        TypeFace const *typeface;

        size_t instancecount;
        TypeFont const *instances[64];
        std::atomic<int> *refcounts[64];
      };

      std::vector<Entry*, StackAllocator<Entry*>> m_entries;

      mutable leap::threadlib::SpinLock m_mutex;
  };
}
