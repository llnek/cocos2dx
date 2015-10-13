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

#if !defined(__XHUDLAYER_H__)
#define __XHUDLAYER_H__

#include "base/ccTypes.h"
#include "XLayer.h"
NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_BEGIN(fusii)

class XLives;
class XLive;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XHUDLayer : public XLayer {
friend class XLives;
protected:

  void AddReplayIcon(not_null<c::MenuItem*>, const c::Vec2& where);
  void AddMenuIcon(not_null<c::MenuItem*>, const c::Vec2& where);

  c::Label* scoreLabel;
  c::Menu* replayBtn;
  XLives* lives;
  int score;

  XHUDLayer();

private:

  NO__CPYASS(XHUDLayer)

public:

  int GetScore() { return score; }

  virtual void ResetAsNew();
  virtual void Reset();

  virtual ~XHUDLayer();

  void UpdateScore(int num);
  bool ReduceLives(int );

  void DisableReplay();
  void EnableReplay();


};





NS_END(fusii)
#endif

