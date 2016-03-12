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

#include "ecs/Engine.h"
#include "n/C.h"

NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GEngine : public ecs::Engine {

  bool maybeOverlap(const f::Box4&);
  s_arr<c::Size, 4> astroSizes;
  s_arr<sstr, 4> astroPools;

public:

  virtual void initEntities();
  virtual void initSystems();

  void createMissiles(int count=16);
  void createLasers(int count=16);
  void createShip();
  void bornShip(ecs::Node*);
  void createAsteroids(int rank);

};


NS_END


