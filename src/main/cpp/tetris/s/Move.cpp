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
#include "n/lib.h"
#include "Move.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
static void scanInput(Gesture *mo) {
  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    mo->right=true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    mo->left=true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
    mo->rotr = true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    mo->rotl = true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_SPACE)) {
    mo->down = true;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  arena = engine->getEntities("n/BlockGrid")[0];
  initKeyOps(arena, CC_CSV(c::Integer, "THROTTLE+WAIT"));
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {

  if (MGMS()->isLive()) {
    auto sh= CC_GEC(ShapeShell, arena, "n/ShapeShell");
    auto dp= CC_GEC(Dropper, arena, "n/Dropper");

    scanner();

    if (cx::timerDone(dp->timer) &&
        NNP(sh->shape)) {
      cx::undoTimer(dp->timer);
      dp->timer= CC_NIL;
      doFall();
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::doFall() {
  auto sh= CC_GEC(ShapeShell, arena, "n/ShapeShell");
  auto bs= CC_GEC(BlockGrid, arena, "n/BlockGrid");
  auto dp= CC_GEC(Dropper, arena, "n/Dropper");
  auto pu= CC_GEC(Pauser, arena, "n/Pauser");
  auto shape= sh->shape;
  auto &emap= bs->grid;

  if (NNP(shape) &&
      ! moveDown(emap, shape)) {

    // lock shape in place
    lock(arena,shape);

    /*
    //TODO: what is this???
    if (ENP(pu->timer)) {
      sh->shape=nullptr;
      shape->bricks.clear();
      delete shape;
    }
    */

    shape->bricks.clear();
    sh->shape= CC_NIL;
    delete shape;

  } else {
    // drop at fast-drop rate
    setDropper(MGML(), dp, dp->dropRate, dp->dropSpeed);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Move::initKeyOps(ecs::Entity *node, int w) {

  auto mo = CC_GEC(Gesture,node,"n/Gesture");

  scanner = cx::throttle([=]() {
      scanInput(mo);
      }, w);

}


NS_END


