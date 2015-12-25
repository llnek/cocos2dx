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

#if !defined(__MOVE_H__)
#define __MOVE_H__

#include "core/XSystem.h"
#include "EFactory.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Move : public f::XSystem<EFactory> {

  virtual const a::SystemType typeId() { return "s/Move"; }

  Move(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);
  virtual int priority() { return a::Move; }
  virtual bool update(float dt);

  void simuMove(a::Node*, a::Node*, float);
  void moveRobot(a::Node*, a::Node*, float);
  void processBall(a::Node*, float);
  void doit(a::Node*, float);
  void notifyServer(a::Node*, int);
  void clamp(c::Sprite*);

  virtual ~Move() {}
  NODFT(Move)
  NOCPYASS(Move)

};

NS_END(pong)
#endif



