// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "GameScene.h"
NS_BEGIN(fusii)

static GameScene* _singleton;

//////////////////////////////////////////////////////////////////////////////
//
GameLayer* GameScene::get() {
  return _singleton->getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
//
GameScene* GameScene::self() {
  return _singleton;
}

//////////////////////////////////////////////////////////////////////////////
//
GameScene::~GameScene() {
  F__LOOP(it, pools) { delete it->second; }
  mc_del_ptr(context);
}

//////////////////////////////////////////////////////////////////////////////
//
GameScene::GameScene() {
  SNPTR(context)
  level = 1;
  bind(this);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Dictionary* GameScene::getCurLevel() {
  return XCFG()->getLevel(s::to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
f::JsonObj* GameScene::getLCfg() {
  return XCFG()->getLevelCfg(s::to_string(level));
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* GameScene::getPool(const stdstr& key) {
  auto it = pools.find(key);
  if (it != pools.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
XPool* GameScene::reifyPool(const stdstr& key) {
  auto it = pools.find(key);
  auto p= mc_new( XPool);
  if (it != pools.end()) {
    delete it->second;
    pools.erase(it);
  }
  pools.insert(S__PAIR(stdstr, XPool*, key, p));
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
void GameScene::resetPools() {
  F__LOOP(it, pools) {
    it->second->clearAll();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GameScene::bind(not_null<GameScene*> m) {
  _singleton=m;
}

//////////////////////////////////////////////////////////////////////////////
//
ws::OdinIO* GameScene::wsock() {
  return context->odin;
}

//////////////////////////////////////////////////////////////////////////////
//
GContext* GameScene::getCtx() { return context; }

//////////////////////////////////////////////////////////////////////////////
//
GMode GameScene::getMode() {
  return context->mode;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GameScene::isOnline() {
  return context->odin != nullptr;
}



NS_END(fusii)

