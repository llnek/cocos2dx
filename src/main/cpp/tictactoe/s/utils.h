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

#if !defined(__UTILS_H__)
#define __UTILS_H__

#define GD_SZ BD_SZ * BD_SZ
#define BD_SZ 3

#include "core/CCSX.h"
#include "n/CObjs.h"

typedef s::array<int, GD_SZ> ArrCells;
typedef s::array<int, BD_SZ> ArrDim;

namespace tttoe {

  const s::array<fusii::Box4, GD_SZ> mapGridPos(float scale = 1.0f);

  const s::vector<ArrDim> mapGoalSpace();

  const stdstr pkFlip(const stdstr& img, bool flip );

  const stdstr xrefImg(int value);

  c::Sprite* drawSymbol(not_null<PlayView*> view,
      float x, float y,
      int value, bool flip=false);

  void prepareSeedData(f::GMode );

};

#endif

