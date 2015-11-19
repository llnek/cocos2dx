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
#include "HUD.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* HUDLayer::realize() {

  this->color = cx::ColorRGB("#5e3178");
  regoAtlas("game-pics");

  initLabels();
  initIcons();

  auto r= cx::reifyMenuBtn("icon_replay.png");
  r->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::onReplay));
  r->setColor(this->color);
  //r->setScale(scale);
  addReplayIcon(r, cx::anchorB());

  auto b= cx::reifyMenuBtn("icon_menu.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::showMenu));
  b->setColor(this->color);
  //b->setScale(scale);
  addMenuIcon(b, cx::anchorB());

  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::initScores() {
  scores.clear();
  scores[1]= 0;
  scores[2]= 0;
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::initLabels() {
  auto soff= CC_CSV(c::Integer, "S_OFF");
  auto tile= CC_CSV(c::Integer, "TILE");
  auto scale= XCFG()->getScale();
  auto cw= cx::center();
  auto wb= cx::visBox();

  //title
  title = cx::reifyBmfLabel( "font.JellyBelly", "");
  title->setScale(scale * 0.6);
  title->setAnchor(cx::anchorT());
  title->setColor(this->color);
  title->setPosition(cw.x, wb.top - 2*tile);
  addItem(title);

  //score1
  score1= cx::reifyBmfLabel("font.SmallTypeWriting", "0");
  score1->setAnchor(cx::anchorTL());
  score1->setColor(cx::white());
  score1->setScale(scale);
  score1->setPosition(tile + soff + 2, wb.top - tile - soff);
  addItem(score1);

  //score2
  score2= cx::reifyBmfLabel( "font.SmallTypeWriting", "0");
  score2->setAnchor(cx::anchorTR());
  score2->setColor(cx::white());
  score2->setScale(scale);
  score2->setPosition(wb.right - tile - soff,
              wb.top - tile - soff);
  addItem(score2);

  // status
  status= cx::reifyBmfLabel( "font.CoffeeBuzzed", "");
  status->setColor(cx::white());
  status->setScale(scale * 0.3);
  status->setPosition(cw.x, wb.bottom + tile * 10);
  addItem(status);

  // result
  result= cx::reifyBmfLabel( "font.CoffeeBuzzed", "");
  result->setColor(cx::white());
  result->setScale(scale * 0.3);
  result->setPosition(cw.x, wb.bottom + tile * 10);
  result->setVisible(false);
  addItem(result);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto hh = cx::getHeight(b) * 0.5;
  auto hw = cx::getWidth(b) * 0.5;
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::showMenu));
  b->setColor(this->color);
  auto menu = cx::MkMenu(b);
  menu->setPosition(wb.right - tile - hw, wb.bottom + tile  + hh);
  addItem(menu, 10);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showTimer() {
  auto ptt= CC_CSV(c::Integer, "PLAYER_THINK_TIME");
  auto tile= CC_CSV(c::Integer,"TILE");
  auto scale= XCFG()->getScale();
  auto cw= cx::center();
  auto wb= cx::visBox();

  // timer is already showing, go away
  if (countDownState) {
    return;
  }

  if (ENP(countDown)) {
    countDown= cx::reifyBmfLabel(
      "font.AutoMission", "");
    countDown->setScale(scale * 0.5);
    countDown->setColor(cx::white());
    countDown->setPosition(cw.x, wb.top - 10*tile);
    countDown->setAnchor(cx::anchorC());
    addItem(countDown);
  }

  countDownState= true;
  countDownValue= ptt;

  showCountDown();

  schedule(this->UpdateTimer, 1.0);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateTimer(float dt) {
  if (!countDownState) { return; } else {
    countDownValue -= 1;
  }
  if (countDownValue < 0) {
    killTimer();
    MGMS()->sendMsg("/player/timer/expired");
  } else {
    showCountDown();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showCountDown(const stdstr& msg) {
  if (NNP(countDown)) {
    countDown->setString(msg);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::killTimer() {
  if (countDownState) {
    unschedule(this->updateTimer);
    showCountDown(" ");
  }
  countDownValue=0;
  countDownState=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int pcolor, int value) {
  scores[pcolor] += value;
  drawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::draw(bool running, int pnum) {
  if (running) {
    drawStatus(pnum);
  } else {
    drawResult(pnum);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame(int winner) {
  status->setVisible(false);
  result->setVisible(true);
  killTimer();
  drawResult(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawXXXText(c::Label* obj, const stdstr& msg) {
  obj->setString(msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawScores() {
  auto s2 = scores[this->play2];
  auto s1 = scores[this->play1];
  auto n2 = s::to_string(s2);
  auto n1 = s::to_string(s1);

  score1->setString(n1);
  score2->setString(n2);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawResult(int pnum) {
  auto msg = XCFG()->getL10NStr("whodraw");

  switch (pnum) {
    case 2:
      msg= XCFG()->getL10NStr("whowin",
          s::vector<stdstr> { this->p2Long});
      break;
    case 1:
      msg= XCFG()->getL10NStr("whowin",
          s::vector<stdstr> { this->p1Long});
      break;
  }

  drawXXXText(result, msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawStatus(int pnum) {
  if (pnum > 0) {
    auto pfx = pnum == 1 ? p1Long : p2Long;
    drawXXXText(status,
      XCFG()->getL10NStr("whosturn",
        s::vector<stdstr> { pfx }));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::regoPlayers(const stdstr& color1,
    const stdstr& p1k, const stdstr& p1n,
    const stdstr& color2, const stdstr& p2k, const stdstr& p2n) {

  scores.insert(CC_PAIR(stdstr,int, color2, 0));
  scores.insert(CC_PAIR(stdstr,int, color1, 0));
  play2= color2;
  play1= color1;
  p2Long= p2n;
  p1Long= p1n;
  p2ID= p2k;
  p1ID= p1k;
  title->setString(p1ID + " / " + p2ID);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::resetAsNew() {
  initScores();
  reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::reset() {
  result->setVisible(false);
  status->setVisible(true);
}


NS_END(tttoe)

