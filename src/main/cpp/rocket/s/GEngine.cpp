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
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(rocket)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto ent= this->reifyNode("Shared", true);
  auto wb=cx::visBox();
  ent->checkin(mc_new(GVars));

  ent= this->reifyNode("RPath", true);
  auto c= RPath::create();
  ent->checkin(c);
  MGML()->addItem(c,1);

  auto r= new Rocket(cx::reifySprite("rocket.png"));
  auto mv= mc_new(RocketMotion);
  r->setPos(wb.cx, wb.top * 0.1);
  rocketReset(r,mv);
  MGML()->addAtlasItem(
      "game-pics",
      r->node, kForeground, kSpriteRocket);
  ent= this->reifyNode("Rocket", true);
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(mv);
  ent->checkin(r);

  MGMS()->reifyPool("Planets");
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {

  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Move,this));

}


NS_END

