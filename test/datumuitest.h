//
// datumuitest.h
//

#pragma once

#include "datum.h"
#include "datum/renderer.h"
#include "datum/ui.h"

//|---------------------- GameState -----------------------------------------
//|--------------------------------------------------------------------------

struct GameState
{
  GameState(StackAllocator<> const &allocator);

  const float fov = 60.0f;
  const float aspect = 1920.0f/1080.0f;

  enum { Startup, Init, Play } mode;

  Camera camera;

  Font const *debugfont;

  AssetManager assets;

  ResourceManager resources;

  RenderContext rendercontext;

  Ui::Context uicontext;

  std::vector<Ui::Item*, StackAllocator<Ui::Item*>> panels;

  size_t resourcetoken = 0;
};

void datumuitest_init(DatumPlatform::PlatformInterface &platform);
void datumuitest_update(DatumPlatform::PlatformInterface &platform, DatumPlatform::GameInput const &input, float dt);
void datumuitest_render(DatumPlatform::PlatformInterface &platform, DatumPlatform::Viewport const &viewport);
