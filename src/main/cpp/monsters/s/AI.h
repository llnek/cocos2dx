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

#include "ecs/System.h"
#include "GEngine.h"
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AILogic : public ecs::System {
  void process(float);

public:

    void spawnMonster(ecs::Entity*, int cost, int count);
    void changeStateForEntity(ecs::Entity*, AIState*);
    void spawnQuirkForEntity(ecs::Entity*);
    void spawnMunchForEntity(ecs::Entity*);
    void spawnZapForEntity(ecs::Entity*);
    
  DECL_PTR(ecs::Entity, _enemy)
  DECL_FZ(humanQuirkValue)
  DECL_FZ(humanZapValue)
  DECL_FZ(humanMunchValue)
  DECL_FZ(humanTotalValue)
  DECL_FZ(aiQuirkValue)
  DECL_FZ(aiZapValue)
  DECL_FZ(aiMunchValue)
  DECL_FZ(aiTotalValue)


  MDECL_SYS_TPID("n/AILogic")
  MDECL_SYS_PRIORITY( 60)
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  AILogic(ecs::Engine *e)
  : System(e)
  {}

};


NS_END



