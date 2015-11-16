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

#if !defined(__XLAYER_H__)
#define __XLAYER_H__

#include "2d/CCSpriteBatchNode.h"
#include "platform/CCCommon.h"
#include "2d/CCLayer.h"
#include "core/Primitives.h"
NS_BEGIN(fusii)

class SpriteBatchNode;
class XScene;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLayer : public c::Layer {
protected:

  s::map<stdstr, c::SpriteBatchNode*> atlases;

  int lastTag;
  int lastZ;

  NO__CPYASS(XLayer)
  XLayer();

public:

  virtual c::SpriteBatchNode* GetAtlas(const stdstr& name);
  virtual void OnQuit(c::Ref*);
  virtual XLayer* Realize();

  c::SpriteBatchNode* RegoAtlas(const stdstr& name,
      const Maybe<int>& = Maybe<int>(),
      const Maybe<int>& = Maybe<int>() );

  virtual void AddAtlasItem(const stdstr& atlas,
      not_null<c::Node*> n,
      const Maybe<int>& = Maybe<int>(),
      const Maybe<int>& = Maybe<int>() );

  virtual void AddItem(not_null<c::Node*> n,
      const Maybe<int>& = Maybe<int>() ,
      const Maybe<int>& = Maybe<int>() );

  void AddAtlasFrame(const stdstr& atlas, const stdstr& n, const c::Vec2& pos);

  void AddFrame(const stdstr& n, const c::Vec2& pos);

  void CenterImage(const stdstr& n, int z= -1);

  void RemoveAtlasAll(const stdstr& atlas) ;
  void RemoveAll();
  void RemoveItem(not_null<c::Node*> n);

  XScene* GetScene();
  int IncIndexZ();

  // tag value
  virtual int GetIID() { return 1; }
  virtual ~XLayer();

};



NS_END(fusii)
#endif

