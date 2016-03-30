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
NS_BEGIN(cocoban)

#define INTARR_SZ 7
typedef s_arr<int,INTARR_SZ> INTXARR;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  virtual ~GVars() { F__LOOP(it,crates) { delete *it; } }
  MDECL_COMP_TPID( "n/GVars" )

  s_arr<INTXARR,INTARR_SZ> levels;
    s_vec<f::NodePtrArray*> crates;
  f::Cell2I playerPos;
  c::Vec2 startTouch;
  c::Vec2 endTouch;
  __decl_iz(swipeTolerance)

};



NS_END




