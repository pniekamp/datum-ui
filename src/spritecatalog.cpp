//
// datum-ui - sprite catalog
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "spritecatalog.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using leap::strlcpy;
using leap::extentof;

namespace Ui
{
  //|---------------------- SpriteCatalog -------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// SpriteCatalog::Constructor ////////////////////////
  SpriteCatalog::SpriteCatalog(ResourceManager &resources, allocator_type const &allocator)
    : m_allocator(allocator),
      m_resources(&resources),
      m_entries(allocator)
  {
  }


  ///////////////////////// SpriteCatalog::add ////////////////////////////////
  void SpriteCatalog::add(leap::string_view name, Asset const *asset)
  {
    assert(asset);

    leap::threadlib::SyncLock lock(m_mutex);

    auto entry = new(allocate<Entry>(m_allocator)) Entry;
    auto namestr = allocate<char>(m_allocator, name.size() + 1);

    entry->name = namestr;
    entry->namelen = name.size();
    strlcpy(namestr, name.data(), name.size() + 1);

    entry->spriteatlas = m_resources->create<Texture>(asset, Texture::Format::SRGBA);

    entry->instancecount = 0;

    for(auto &refcount : entry->refcounts)
    {
      refcount = new(allocate<std::atomic<int>>(m_allocator)) std::atomic<int>;
    }

    m_entries.push_back(entry);
  }


  ///////////////////////// SpriteCatalog::count //////////////////////////////
  size_t SpriteCatalog::count() const
  {
    leap::threadlib::SyncLock lock(m_mutex);

    return m_entries.size();
  }


  ///////////////////////// SpriteCatalog::entry //////////////////////////////
  leap::string_view SpriteCatalog::entry(size_t i) const
  {
    leap::threadlib::SyncLock lock(m_mutex);

    return leap::string_view(m_entries[i]->name, m_entries[i]->namelen);
  }


  ///////////////////////// SpriteCatalog::find ///////////////////////////////
  Texture const *SpriteCatalog::find(leap::string_view name) const
  {
    leap::threadlib::SyncLock lock(m_mutex);

    for(auto &entry : m_entries)
    {
      if (leap::string_view(entry->name, entry->namelen) == name)
      {
        return entry->spriteatlas;
      }
    }

    return nullptr;
  }


  ///////////////////////// SpriteCatalog::find ///////////////////////////////
  Sprite SpriteCatalog::find(leap::string_view name, Rect2 region, Vec2 align) const
  {
    leap::threadlib::SyncLock lock(m_mutex);

    Vec4 extent = Vec4(region.min.x, region.min.y, region.max.x - region.min.x, region.max.y - region.min.y);

    for(auto &entry : m_entries)
    {
      if (leap::string_view(entry->name, entry->namelen) == name)
      {
        for(size_t i = 0; i < entry->instancecount; ++i)
        {
          if (entry->instances[i]->extent == extent && entry->instances[i]->align == align)
            return Sprite(entry->instances[i], entry->refcounts[i]);
        }

        if (entry->instancecount < extentof(entry->instances))
        {
          size_t i = entry->instancecount++;

          entry->instances[i] = m_resources->create<SpriteImage>(entry->spriteatlas, region, align);

          entry->refcounts[i]->store(0);

          return Sprite(entry->instances[i], entry->refcounts[i]);
        }

        LOG_ONCE("Sprite Instances Exhausted")
      }
    }

    return {};
  }


  ///////////////////////// request /////////////////////////////////////////
  void SpriteCatalog::request(DatumPlatform::PlatformInterface &platform, SpriteAtlas const *spriteatlas)
  {
    m_resources->request(platform, spriteatlas);
  }


  ///////////////////////// request /////////////////////////////////////////
  void SpriteCatalog::request(DatumPlatform::PlatformInterface &platform, Sprite const &sprite)
  {
    m_resources->request(platform, sprite);
  }


  ///////////////////////// sweep_resources ///////////////////////////////////
  void SpriteCatalog::sweep_resources()
  {
    leap::threadlib::SyncLock lock(m_mutex);

    for(auto &entry : m_entries)
    {
      for(size_t i = 0; i < entry->instancecount; )
      {
        if (entry->refcounts[i]->load() == 0)
        {
          m_resources->release(entry->instances[i]);

          entry->instances[i] = entry->instances[entry->instancecount - 1];
          swap(entry->refcounts[i], entry->refcounts[entry->instancecount - 1]);

          entry->instancecount -= 1;
        }
        else
          ++i;
      }
    }
  }
}
