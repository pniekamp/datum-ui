//
// datumuitest.cpp
//

#include "datumuitest.h"
#include "fallback.h"
#include "datum/ui/compiler.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;


///////////////////////// GameState::Constructor ////////////////////////////
GameState::GameState(StackAllocator<> const &allocator)
  : assets(allocator),
    resources(assets, allocator),
    uicontext(resources, allocator),
    panels(allocator)
{
}


///////////////////////// game_init /////////////////////////////////////////
void datumuitest_init(PlatformInterface &platform)
{
  GameState &state = *new(allocate<GameState>(platform.gamememory)) GameState(platform.gamememory);

  initialise_asset_system(platform, state.assets, 64*1024, 128*1024*1024);

  initialise_resource_system(platform, state.resources, 2*1024*1024, 8*1024*1024, 64*1024*1024, 1);

  initialise_render_context(platform, state.rendercontext, 16*1024*1024, 0);

  state.camera.set_projection(state.fov*pi<float>()/180.0f, state.aspect);

  auto core = state.assets.load(platform, "core.pack");

  if (!core)
    throw runtime_error("Core Assets Load Failure");

  if (core->magic != CoreAsset::magic || core->version != CoreAsset::version)
    throw runtime_error("Core Assets Version Mismatch");

  state.debugfont = state.resources.create<Font>(state.assets.find(CoreAsset::debug_font));

  auto testpack = state.assets.load(platform, "test.pack");

  if (!testpack)
    throw runtime_error("Test Assets Load Failure");

  state.uicontext.add_resources(platform, state.assets, testpack);

  auto b = parse_ui(load_ui("./test.ui"));

  codegen_ui("test_p.h", "Test", b.data(), b.size());

  LoaderItemList items(platform.gamescratchmemory);
  auto c = create_ui(state.uicontext, b.data(), b.size(), &items, platform.gamescratchmemory);

  if (c != nullptr)
  {
    c->x = 10;
    c->y = 10;
    c->width = 300;
    c->height = 450;

    state.panels.push_back(c);
  }

  state.mode = GameState::Startup;
}


///////////////////////// game_resize ///////////////////////////////////////
void datumuitest_resize(PlatformInterface &platform, Viewport const &viewport)
{
  GameState &state = *static_cast<GameState*>(platform.gamememory.data);

  if (state.rendercontext.ready)
  {
    RenderParams renderparams;
    renderparams.width = viewport.width;
    renderparams.height = viewport.height;
    renderparams.aspect = state.aspect;
    renderparams.fogdensity = 0.0f;
    renderparams.ssaoscale = 0.0f;

    prepare_render_pipeline(state.rendercontext, renderparams);
  }

  state.uicontext.fontcatalog.sweep_resources();
  state.uicontext.spritecatalog.sweep_resources();

//  state.viewport = Rect2(Vec2(viewport.x, viewport.y), Vec2(viewport.x + viewport.width, viewport.y + viewport.height));
}


///////////////////////// game_update ///////////////////////////////////////
void datumuitest_update(DatumPlatform::PlatformInterface &platform, DatumPlatform::GameInput const &input, float dt)
{
  BEGIN_TIMED_BLOCK(Update, Color3(1.0f, 1.0f, 0.4f))

  GameState &state = *static_cast<GameState*>(platform.gamememory.data);

  if (state.mode == GameState::Startup)
  {
    asset_guard lock(state.assets);

    int ready = 0, total = 0;

    request(platform, state.resources, state.debugfont, &ready, &total);
    request(platform, state.rendercontext, &ready, &total);

    for(auto &panel : state.panels)
    {
      request(platform, state.uicontext, panel, &ready, &total);
    }

    if (ready == total)
    {
      state.mode = GameState::Play;
    }
  }

  if (state.mode == GameState::Play)
  {
    bool inputaccepted = state.uicontext.pressitem;

    update_debug_overlay(input, &inputaccepted);

    update_ui_overlay(state.uicontext, state.panels, input, dt, &inputaccepted);
  }

  state.resourcetoken = state.resources.token();

  END_TIMED_BLOCK(Update)
}


///////////////////////// game_render ///////////////////////////////////////
void datumuitest_render(DatumPlatform::PlatformInterface &platform, DatumPlatform::Viewport const &viewport)
{
  BEGIN_FRAME()

  GameState &state = *static_cast<GameState*>(platform.gamememory.data);

  BEGIN_TIMED_BLOCK(Render, Color3(0.0f, 0.2f, 1.0f))

  RenderParams renderparams;
  renderparams.width = viewport.width;
  renderparams.height = viewport.height;
  renderparams.aspect = state.aspect;
  renderparams.fogdensity = 0.0f;
  renderparams.ssaoscale = 0.0f;

  if (state.mode == GameState::Startup)
  {
    if (prepare_render_context(platform, state.rendercontext, state.assets))
    {
      prepare_render_pipeline(state.rendercontext, renderparams);
    }

    render_fallback(state.rendercontext, viewport, embeded::logo.data, embeded::logo.width, embeded::logo.height);
  }

  if (state.mode == GameState::Play)
  {
    auto &camera = state.camera;

    RenderList renderlist(platform.renderscratchmemory, 8*1024*1024);

    for(auto &action : state.uicontext.actions)
    {
      if (action.id == 5)
      {
        if (state.uicontext.activeitem != state.panels.back())
        {
          raise(state.uicontext, state.uicontext.activeitem, state.panels);
        }
      }

      if (action.id == 5 && action.op == Ui::DragButton::Dragged)
      {
        auto handle = static_cast<Ui::DragButton*>(action.item);

        action.ui->x = clamp(action.ui->x + handle->dx, 0.0f, viewport.width - action.ui->width);
        action.ui->y = clamp(action.ui->y + handle->dy, 0.0f, viewport.height - action.ui->height);
      }

      if (action.id == 6 && action.op == Ui::DragButton::Dragged)
      {
        auto handle = static_cast<Ui::DragButton*>(action.item);

        action.ui->width = clamp(action.ui->width + handle->dx, 100.0f, viewport.width - action.ui->x);
        action.ui->height = clamp(action.ui->height + handle->dy, 60.0f, viewport.height - action.ui->y);
      }
    }

    render_ui_overlay(state.rendercontext, state.resources, renderlist, viewport, state.uicontext, state.panels);

    render_debug_overlay(state.rendercontext, state.resources, renderlist, viewport, state.debugfont);

    render(state.rendercontext, viewport, camera, renderlist, renderparams);
  }

  state.resources.release(state.resourcetoken);

  END_TIMED_BLOCK(Render)
}
