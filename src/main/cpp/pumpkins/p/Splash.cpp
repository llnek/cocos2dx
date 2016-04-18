// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto b = cx::reifyMenuBtn("TWplaybtn.png");
  XCFG()->fit(b);
  auto wb= cx::visBox();

  CC_POS2(b, wb.cx, wb.top * 0.2);
  b->setCallback([=](c::Ref*){
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  centerImage("title.bg");
  addItem(cx::mkMenu(b));

}


NS_END


