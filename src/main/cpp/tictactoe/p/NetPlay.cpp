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

#include "ui/UITextField.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/JSON.h"
#include "NetPlay.h"
#include "Game.h"
#include "Menu.h"
#include "s/utils.h"

NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()

const int USER_TAG = (int) 'u';
const int PSWD_TAG = (int) 'p';

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {

  void networkEvent(ws::OdinEvent*);
  void sessionEvent(ws::OdinEvent*);
  void odinEvent(ws::OdinEvent*);

  void onPlayReply(ws::OdinEvent*);
  void showWaitOthers();
  void onStart(ws::OdinEvent*);

  void onCancel(c::Ref* );
  void onLogin(c::Ref*);

  DECL_PTR(ws::OdinIO, odin)
  DECL_IZ(player)

  STATIC_REIFY_LAYER(UILayer)
  MDECL_DECORATE()

  virtual ~UILayer() {
    ws::disconnect(odin);
  }
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::showWaitOthers() {

  auto qn= cx::reifyBmfLabel("font.OCR", gets("waitother"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb = cx::visBox();

  removeAll();

  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
  qn->setOpacity(0.9f * 255);
  addItem(qn);

  auto b1= cx::reifyMenuBtn("cancel.png");
  auto menu = cx::mkMenu(b1);
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onCancel));
  menu->setPosition(cw.x, wb.top * 0.1f);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onStart(ws::OdinEvent *evt) {
  auto m= f::GMode::NET;
  auto obj= fmtGameData(m);

  obj["ppids"] = evt->doco["source"]["ppids"];
  obj["pnum"]= j::json(player);

  auto ctx = (NPCX*) getSceneX()->getCtx();
  ctx->yes(odin,obj);
  SNPTR(odin)
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onCancel(c::Ref*) {
  auto f= [=]() {
      cx::runScene(XCFG()->prelude(), getDelay()); };
  auto m = MMenu::reify(mc_new_1(MCX, f));

  ws::disconnect(odin);
  SNPTR(odin)

  cx::runScene( m, getDelay());
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlayReply(ws::OdinEvent *evt) {
  player= JS_INT(  evt->doco["pnum"]);
  CCLOG("player %d: ok", player);
  showWaitOthers();
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::networkEvent(ws::OdinEvent *evt) {
  switch (evt->code) {
    case ws::EType::PLAYER_JOINED:
      //TODO
      //CCLOG("another player joined room: ", evt.source.puid);
    break;
    case ws::EType::START:
      CCLOG("play room is ready, game can start");
      odin->cancelAll();
      onStart(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::sessionEvent(ws::OdinEvent *evt) {
  switch (evt->code) {
    case ws::EType::PLAYREQ_OK:
      onPlayReply(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::odinEvent(ws::OdinEvent *evt) {
  //CCLOG("odin event = %p", evt);
  switch (evt->type) {
    case ws::MType::NETWORK:
      networkEvent(evt);
    break;
    case ws::MType::SESSION:
      sessionEvent(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onLogin(c::Ref* ) {
  auto u= getChildByTag( USER_TAG);
  auto p= getChildByTag( PSWD_TAG);
  auto uid = ((c::ui::TextField*) u)->getString();
  auto pwd = ((c::ui::TextField*) p)->getString();

  //TODO: fix url
  auto wsurl = XCFG()->getWSUrl();
  auto game = XCFG()->getGameId();

  if (uid.length() > 0 && pwd.length() > 0) {
    odin= ws::reifyPlayRequest(game, uid, pwd);
    odin->listen([=](ws::OdinEvent *e) {
        this->odinEvent(e);
        });
    ws::connect(odin, wsurl);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto qn= cx::reifyBmfLabel("font.OCR", gets("signinplay"));
  auto wz= cx::visRect();
  auto cw= cx::center();
  auto wb= cx::visBox();
  int tag;

  centerImage("game.bg");
  incIndexZ();

  // text msg
  qn->setScale(XCFG()->getScale() * 0.3f);
  qn->setPosition(cw.x, wb.top * 0.75f);
  qn->setOpacity(0.9f*255);
  addItem(qn);

  // editbox for user
  auto uid = c::ui::TextField::create();
  auto bxz = cx::calcSize("ok.png");
  uid->setMaxLengthEnabled(true);
  uid->setMaxLength(16);
  uid->setTouchEnabled(true);
  uid->setFontName( "Arial");
  uid->setFontSize( 18);
  uid->setPlaceHolder(gets("userid"));
  uid->setPosition(c::Vec2(cw.x, cw.y+bxz.height*0.5f+2));
  addItem(uid, lastZ, USER_TAG);

  // editbox for password
  auto pwd = c::ui::TextField::create();
  pwd->setPasswordEnabled(true);
  pwd->setPasswordStyleText("*");
  pwd->setTouchEnabled(true);
  pwd->setMaxLength(16);
  pwd->setFontName( "Arial");
  pwd->setFontSize( 18);
  pwd->setPlaceHolder( gets("passwd"));
  pwd->setPosition(c::Vec2(cw.x, cw.y-bxz.height*0.5f-2));
  addItem(pwd, lastZ, PSWD_TAG);

  // btns
  auto b1= cx::reifyMenuBtn("continue.png");
  auto b2= cx::reifyMenuBtn("cancel.png");
  auto menu= cx::mkVMenu(s::vector<c::MenuItem*> {b1, b2});

  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onLogin));

  b2->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onCancel));

  menu->setPosition(cw.x, wb.top * 0.1f);
  addItem(menu);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void NetPlay::decorate() {
  UILayer::reify(this);
}


NS_END(tttoe)


