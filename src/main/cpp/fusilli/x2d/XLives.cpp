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

#include "core/CCSX.h"
#include "XHUDLayer.h"
#include "XLive.h"
#include "XLives.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
void XLives::Reduce(int x) {
  while (x > 0) {
    if (icons.size() > 0) {
      auto it= icons.back();
      it->removeFromParent();
      icons.pop_back();
    }
    --x;
    --curLives;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::Reset() {
  for (auto it= icons.begin(); it != icons.end(); ++it) {
    auto n= *it;
    n->removeFromParent();
  }
  curLives = totalLives;
  icons.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::Resurrect() {
  Reset();
  DrawLives();
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::DrawLives() {
//  auto gap=2;
  float y;
  float x;

  for (int n = 0; n < curLives; ++n) {
    auto v= XLive::Create(frameId);
    if (n==0) {
      lifeSize = v->getContentSize();
      y= refPt.y - lifeSize.height * 0.5;
      x= refPt.x + lifeSize.width * 0.5;
    }
    v->setPosition(x,y);
    addChild(v);
    icons.push_back(v);
    if (this->dir > 0) {
      x += lifeSize.width * 1.2f;
    } else {
      x -= lifeSize.width - 1.2f;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::Realize(const stdstr& frame,
    int lives,
    float x, float y, int dir) {

  totalLives = lives;
  curLives = 0;
  this->dir = dir;
  refPt= c::Vec2(x,y);
  frameId = frame;
  Reset();
  DrawLives();
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::XLives() {
  totalLives = 0;
  curLives = 0;
  dir = 1;
}

NS_END(fusii)
