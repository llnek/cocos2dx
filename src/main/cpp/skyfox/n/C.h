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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(skyfox)

enum {
  kSpriteBomb,
  kSpriteShockwave,
  kSpriteMeteor,
  kSpriteHealth,
  kSpriteHalo,
  kSpriteSparkle,
  kSpriteUfo,
  kSpriteRay
};

enum {
  kBackground,
  kMiddleground,
  kForeground
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cloud : public f::CPixie {
  Cloud(not_null<c::Node*> s)
  : CPixie(s) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Meteor : public f::CPixie {
  Meteor(not_null<c::Node*> s)
  : CPixie(s) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Health : public f::CPixie {
  Health(not_null<c::Node*> s)
  : CPixie(s) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::CPixie {
  Bomb(not_null<c::Node*> s)
  : CPixie(s) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ufo : public f::CPixie {
  Ufo(not_null<c::Node*> s)
  : CPixie(s) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )

  DECL_FZ(difficultyInterval)
  DECL_FZ(difficultyTimer)

  DECL_FZ(meteorInterval)
  DECL_FZ(meteorTimer)
  DECL_FZ(meteorSpeed)

  DECL_FZ(healthInterval)
  DECL_FZ(healthTimer)
  DECL_FZ(healthSpeed)

  DECL_FZ(ufoInterval)
  DECL_FZ(ufoTimer)
  DECL_BF(ufoKilled)

  DECL_IZ(shockwaveHits)
  DECL_FZ(energy)

  DECL_PTR(c::RepeatForever,rotateSprite)
  DECL_PTR(c::RepeatForever,swingHealth)
  DECL_PTR(c::RepeatForever,blinkRay)
  DECL_PTR(c::Sequence,shockwaveSequence)
  DECL_PTR(c::Sequence,groundHit)
  DECL_PTR(c::Sequence,explosion)
  DECL_PTR(c::Sprite,shockWave)
  DECL_PTR(c::ScaleTo,growBomb)
  DECL_PTR(c::Animate,ufoAnimation)

  s_map<ecs::Node*,ecs::Node*> fallingObjects;
};



NS_END




