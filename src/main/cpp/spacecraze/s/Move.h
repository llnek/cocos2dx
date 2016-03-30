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

#include "GEngine.h"
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Move : public ecs::System {

  void firePlayerBullet(Ship*, float);
  void fireBombs(ecs::Node*);
  void processMissiles(float);
  void processBombs(float);
  void processAliens(float);
  void processShip(float);

  __decl_ptr(ecs::Node, _aliens)
  __decl_ptr(ecs::Node, _arena)
  __decl_ptr(ecs::Node, _ship)

public:

  MDECL_SYS_PRIORITY(ecs::Move)
  MDECL_SYS_TPID("s/Move")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Move(ecs::Engine *e)
    : System(e) {
  }

};



NS_END





