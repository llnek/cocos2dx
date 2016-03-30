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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

#include "n/Fairytale.h"
#include "n/Tower.h"
#include "n/Dragon.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(flappy)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }
  void onEnd();

  __decl_ptr(ecs::Node, _shared)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual bool onMouseStart(const c::Vec2&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _shared = _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  // set the roof of the castle
  ss->castleRoof = 100;

  // create & init all managers
  ss->towers= new Tower(this);
  ss->towers->init();

  ss->dragon= new Dragon(ss,this);
  ss->dragon->init();

  ss->fairytale= new Fairytale(this);
  ss->fairytale->init();
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const c::Vec2 &loc) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");

  ss->hasGameStarted = true;
  getHUD()->getReady();

  // inform DragonManager that the game has started
  ss->dragon->onGameStart();

  // fly dragon...fly!!!
  ss->dragon->dragonFlap();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {
  this->setOpacity(255 * 0.1);
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(),4);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  regoAtlas("game-pics", E_LAYER_BG + 1);
  regoAtlas("dhtex", E_LAYER_BG + 1);

  _engine = mc_new(GEngine);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if ("/game/player/lose"==topic) {
    y->onEnd();
  }

  if ("/game/player/earnscore"==topic) {
    auto msg= (j::json*) m;
    y->getHUD()->updateScore(JS_INT(msg->operator[]("score")));
  }

  if (""==topic) {
  }


  if (""==topic) {
  }


}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}


NS_END




