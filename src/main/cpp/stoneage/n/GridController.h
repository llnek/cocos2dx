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
#include "core/ComObj.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GridController : public f::Component {

  static GridController* create();

  GridController() {}

  bool checkGridMatches();

  void checkTypeMatch(int c, int r);

  void addMatches(matches);

  void find(np, array);

  void findGemAtPosition(position);

  void selectStartGem(touchedGem);

  void selectTargetGem(touchedGem);

  void onTouchUp(touch);

  void onTouchMove(touch);

  void isValidTarget(px, py, touch);

  s_vec<> matchArray;
  this->enabled = true;
  this->touchDown = false;

};



NS_END



