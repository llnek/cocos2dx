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

#if !defined(__MSGBOX_H__)
#define __MSGBOX_H__


#include "platform/CCCommon.h"
#include "XScene.h"

NS_ALIAS(c,cocos2d)
NS_ALIAS(s,std)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////
//
class CC_DLL MsgBox : public XScene {
friend class MsgBoxLayer;
protected:

  const stdstr& GetMsg() { return textMsg; }

  void SetAction(c::CallFunc* cb);
  void SetMsg(const stdstr&);
  void OnYes(c::Ref*);

  c::CallFunc* action;
  stdstr textMsg;

private:

  CREATE_FUNC(MsgBox)
  NO__CPYASS(MsgBox)
  MsgBox();

public:

  static MsgBox* CreateWithAction(c::CallFunc*, const stdstr&);
  static MsgBox* CreateWithMsg(const stdstr&);

  virtual XScene* Realize();
  virtual ~MsgBox();

};



NS_END(fusii)
#endif

