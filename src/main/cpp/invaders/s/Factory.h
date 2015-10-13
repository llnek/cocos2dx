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

#if !defined(__FACTORY_H__)
#define __FACTORY_H__

#include "core/XPool.h"
#include "ash/Engine.h"
NS_ALIAS(f, fusii)
NS_ALIAS(a, ash)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Factory {
protected:

  c::Dictionary* state;
  a::Engine* engine;
  f::ComObj* player;

private:

  NO__CPYASS(Factory)
  Factory();

public:

  Factory(not_null<a::Engine*>, not_null<c::Dictionary*> options);
  void ReifyMissiles(int count= 36);
  void ReifyExplosions(int count = 24);
  void ReifyBombs(int count = 24);

  const c::Size CalcImgSize(const stdstr& img);
  c::Dictionary* GetRankInfo(int r);

  void FillSquad(not_null<f::XPool*> );
  a::Entity* ReifyAliens();
  void BornShip();
  a::Entity* ReifyShip();

  virtual ~Factory();
};


NS_END(invaders)
#endif


