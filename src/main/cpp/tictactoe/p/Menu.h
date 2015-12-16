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

#if !defined(__MENU_H__)
#define __MENU_H__

#include "2d/CCActionInstant.h"
#include "x2d/XScene.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL MCX : public f::SCTX {
  MCX(VOIDFN b) { back=b; }
  VOIDFN back;
};
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MMenu : public f::XScene {
public:

  STATIC_REIFY_SCENE_CTX(MMenu)

  virtual void decorate();

  virtual ~MMenu() {}
  MMenu() {}

  NOCPYASS(MMenu)
};



NS_END(tttoe)
#endif


