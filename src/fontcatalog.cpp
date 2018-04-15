//
// datum-ui - font catalog
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "fontcatalog.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using leap::strlcpy;
using leap::extentof;

namespace Ui
{
  //|---------------------- FontCatalog ---------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// FontCatalog::Constructor //////////////////////////
  FontCatalog::FontCatalog(ResourceManager &resources, allocator_type const &allocator)
    : m_allocator(allocator),
      m_resources(&resources),
      m_entries(allocator)
  {
  }


  ///////////////////////// FontCatalog::add //////////////////////////////////
  void FontCatalog::add(leap::string_view name, Asset const *asset)
  {
    assert(asset);

    leap::threadlib::SyncLock lock(m_mutex);

    auto entry = new(allocate<Entry>(m_allocator)) Entry;
    auto namestr = allocate<char>(m_allocator, name.size() + 1);

    entry->name = namestr;
    entry->namelen = name.size();
    strlcpy(namestr, name.data(), name.size() + 1);

    entry->typeface = m_resources->create<TypeFace>(asset);

    entry->instancecount = 0;

    for(auto &refcount : entry->refcounts)
    {
      refcount = new(allocate<std::atomic<int>>(m_allocator)) std::atomic<int>;
    }

    m_entries.push_back(entry);
  }


  ///////////////////////// FontCatalog::find /////////////////////////////////
  TypeFace const *FontCatalog::find(leap::string_view name) const
  {
    leap::threadlib::SyncLock lock(m_mutex);

    for(auto &entry : m_entries)
    {
      if (leap::string_view(entry->name, entry->namelen) == name)
      {
        return entry->typeface;
      }
    }

    return nullptr;
  }


  ///////////////////////// FontCatalog::find /////////////////////////////////
  Font FontCatalog::find(leap::string_view name, float pixelheight) const
  {
    leap::threadlib::SyncLock lock(m_mutex);

    for(auto &entry : m_entries)
    {
      if (leap::string_view(entry->name, entry->namelen) == name)
      {
        for(size_t i = 0; i < entry->instancecount; ++i)
        {
          if (fcmp(entry->instances[i]->pixelheight, pixelheight, 0.05f))
            return Font(entry->instances[i], entry->refcounts[i]);
        }

        if (entry->instancecount < extentof(entry->instances))
        {
          size_t i = entry->instancecount++;

          entry->instances[i] = m_resources->create<TypeFont>(entry->typeface, pixelheight);

          entry->refcounts[i]->store(0);

          return Font(entry->instances[i], entry->refcounts[i]);
        }

        LOG_ONCE("Typeface Instances Exhausted")
      }
    }

    return {};
  }


  ///////////////////////// FontCatalog::find /////////////////////////////////
  Font FontCatalog::find(TypeFace const *typeface, float pixelheight) const
  {
    leap::threadlib::SyncLock lock(m_mutex);

    for(auto &entry : m_entries)
    {
      if (entry->typeface == typeface)
      {
        for(size_t i = 0; i < entry->instancecount; ++i)
        {
          if (fcmp(entry->instances[i]->pixelheight, pixelheight, 0.05f))
            return Font(entry->instances[i], entry->refcounts[i]);
        }

        if (entry->instancecount < extentof(entry->instances))
        {
          size_t i = entry->instancecount++;

          entry->instances[i] = m_resources->create<::TypeFont>(entry->typeface, pixelheight);

          entry->refcounts[i]->store(0);

          return Font(entry->instances[i], entry->refcounts[i]);
        }

        LOG_ONCE("Typeface Instances Exhausted")
      }
    }

    return {};
  }


  ///////////////////////// request_resources /////////////////////////////////
  bool FontCatalog::request_resources(DatumPlatform::PlatformInterface &platform)
  {
    leap::threadlib::SyncLock lock(m_mutex);

    bool ready = true;

    for(auto &entry : m_entries)
    {
      m_resources->request(platform, entry->typeface);

      for(size_t i = 0; i < entry->instancecount; ++i)
      {
        m_resources->request(platform, entry->instances[i]);

        ready &= entry->instances[i]->ready();
      }

      ready &= entry->typeface->ready();
    }

    return ready;
  }


  ///////////////////////// sweep_resources ///////////////////////////////////
  void FontCatalog::sweep_resources()
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


///////////////////////// request ///////////////////////////////////////////
void request(DatumPlatform::PlatformInterface &platform, ResourceManager &resources, Ui::FontCatalog &fonts, int *ready, int *total)
{

}
