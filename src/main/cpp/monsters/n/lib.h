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
#include "ecs/Engine.h"

NS_BEGIN(monsters)
class GEngine;

#define CCRANDOM_X_Y(__X__, __Y__) (((__Y__) - (__X__)) * (rand() / (float)0xffffffff) + (__X__))
#define OTHER_TEAM(__X__) (__X__ == 1 ? 2 : 1)

#define COST_QUIRK  10
#define COST_ZAP    25
#define COST_MUNCH  50


s_vec<ecs::Entity*> getEntsOnTeam(ecs::Engine*, int team,  const ecs::COMType&);

ecs::Entity* closestEntOnTeam(ecs::Engine*, ecs::Entity*, int team);

s_vec<ecs::Entity*> entsWithinRange(ecs::Engine*, ecs::Entity*, float range, int team);

ecs::Entity* playerForTeam(ecs::Engine*, int team);

void createMonsters(ecs::Engine*, int cost, int team, int count);


NS_END




