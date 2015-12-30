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

#if !defined(__GAMELAYER_H__)
#define __GAMELAYER_H__

#include "base/CCEventKeyboard.h"
#include "core/ComObj.h"
#include "ash/Engine.h"
#include "XLayer.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GameLayer : public XLayer {
protected:

  virtual bool onTouchBegan(c::Touch *t, c::Event *e);
  virtual void onTouchMoved(c::Touch *t, c::Event *e);

  virtual void onKeyReleased(KEYCODE k, c::Event*);
  virtual void onKeyPressed(KEYCODE k, c::Event*);

  virtual void onTouchMotion(ComObj*,
      const c::Vec2&, const c::Vec2&);
  virtual void onMouseMotion(ComObj*,
      const c::Vec2&, const c::Vec2&);

  virtual void update(float);

  void disableListeners();
  void enableListeners();

  DECL_PTR(c::EventListenerMouse ,_mouseListener)
  DECL_PTR(a::Engine ,engine)
  DECL_BF(_mouseEnabled)

  s_vec<ComObj*> motionees;
  s_arr<bool, 256> keys;

  virtual void onMouseScroll(c::Event*);
  virtual void onMouseDown(c::Event*);
  virtual void onMouseUp(c::Event*);
  virtual void onMouseMove(c::Event*);

public:

  bool isMouseEnabled() { return _mouseEnabled; }
  void setMouseEnabled(bool);
  bool keyPoll(KEYCODE);

  virtual ~GameLayer();
  GameLayer();

  NOCPYASS(GameLayer)
};

#define DECL_GETLAYER(T,n,x) \
  virtual T* n() \
    { return static_cast<T*>(getSceneX()->getLayer(x)); }



NS_END(fusii)
#endif

