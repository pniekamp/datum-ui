//
// datum-ui - control
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "control.h"
#include "context.h"
#include "datum/debug.h"

using namespace std;
using namespace lml;
using namespace DatumPlatform;

namespace Ui
{
  //|---------------------- Control -------------------------------------------
  //|--------------------------------------------------------------------------

  ///////////////////////// update_item ///////////////////////////////////////
  template<>
  void Context::update_item<Control>(Node *node, GameInput const &input, float dt)
  {    
    auto item = item_cast<Control>(node);

    item->hovered = (!inputaccepted && hoveritem == item) || (pressitem == item);
  }

  ///////////////////////// prepare_item //////////////////////////////////////
  template<>
  void Context::prepare_item<Control>(Node *node)
  {
    prepare_item<Item>(node);

    auto item = item_cast<Control>(node);

    item->fontheight = item->scale * item->fontheight;

    if (item->font && item->fontheight != 0 && !fcmp(item->font->pixelheight, item->fontheight, 0.05f))
    {
      item->font = fontcatalog.find(item->font->typeface, item->fontheight);
    }
  }

}
