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

NS_BEGIN(astros)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL AI : public ecs::System {

    DECL_PTR(c::DelayTime,_astroTimer)
  DECL_PTR(ecs::Node, _shared)
  void parallex(float);
  void process(float);
    void spawnAstro(float);
public:

  MDECL_SYS_PRIORITY( ecs::AI)
  MDECL_SYS_TPID("n/AI")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  AI(ecs::Engine *e)
  : System(e)
  {}

};


NS_END



