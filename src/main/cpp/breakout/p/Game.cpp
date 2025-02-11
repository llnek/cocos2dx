// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "Game.h"
#include "MMenu.h"
#include "HUD.h"
#include "Ende.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onInited();
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchMotion(c::Touch*);

  void onPlayerKilled();
  void spawnPlayer();
  void onEnd();
  void showMenu();

  __decl_ptr(ecs::Node, _paddle)
  __decl_ptr(ecs::Node, _ball)
  __decl_ptr(ecs::Node, _arena)

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
  auto p= CC_GEC(f::CPixie, _paddle,"f/CPixie");
  auto pos= p->getPosition();
  if (loc.y <= pos.y) {
    CC_POS2(p, loc.x, pos.y);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  auto p= CC_GEC(f::CPixie, _paddle,"f/CPixie");
  auto y= p->getPositionY();
  return tap.y <= y;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *t) {
  return onMouseStart( t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *t) {
  onMouseMotion(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _paddle= _engine->getNodes("f/CGesture")[0];
  _ball= _engine->getNodes("f/CAutoma")[0];
  _arena= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::showMenu() {
  auto x= new MCX([=]() { cx::pop(); });
  cx::pushEx(MMenu::reify(x));
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decoUI() {
  _engine = mc_new(GEngine);
  centerImage("game.bg");
  regoAtlas("game-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::spawnPlayer() {
  SCAST(GEngine*, _engine)->bornPaddle(_paddle,_ball);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPlayerKilled() {
  if (getHUD()->reduceLives(1)) {
    onEnd();
  } else {
    spawnPlayer();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &t, void *m) {
  auto y = (GLayer*) getLayer(2);

  if (t == "/game/bricks/killed") {
    auto msg = (j::json*)m;
    //y->onBrickKilled(msg);
  }

  if (t == "/game/players/killed") {
    y->onPlayerKilled();
  }

  if (t == "/game/players/earnscore") {
    auto msg = (j::json*)m;
    y->getHUD()->updateScore(
      JS_INT(msg->operator[]("value")));
  }

  if (t == "/hud/showmenu") {
    y->showMenu();
  }

}

//////////////////////////////////////////////////////////////////////////////
//
const f::Box4 Game::getEnclosureBox() {
  auto cfg= MGMS()->getLCfg()->getValue();
  auto tile = CC_CSV(c::Float, "TILE");
  auto wb= cx::visBox();

  return f::Box4(
      wb.top - JS_INT(cfg["TOP"]) * tile,
      wb.right - tile,
      tile,
      tile);
}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decoUI() {
  HUDLayer::reify(this,3);
  GLayer::reify(this,2);
  play();
}


NS_END



