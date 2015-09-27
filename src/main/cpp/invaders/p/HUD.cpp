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

#include "core/XConfig.h"
#include "2d/CCLabel.h"
#include "core/CCSX.h"
#include "x2d/XLives.h"
#include "HUD.h"
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(f, fusii)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
HUDLayer::~HUDLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
HUDLayer::HUDLayer() {
  SNPTR(m_scoreLabel)
}

void HUDLayer::ShowMenu(c::Ref* r) {

}

void HUDLayer::OnReplay(c::Ref* r) {

}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* HUDLayer::Realize() {

  auto color=  c::Color3B::WHITE;//cx::White();
  auto scale=1;

  auto r= cx::CreateMenuBtn("#icon_replay.png");
  r->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::OnReplay));
  r->setVisible(false);
  r->setColor(color);
  //r->setScale(scale);
  AddReplayIcon(r, cx::AnchorB());

  auto b= cx::CreateMenuBtn("#icon_menu.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::ShowMenu));
  b->setColor(color);
  //b->setScale(scale);
  AddMenuIcon(b, cx::AnchorB());

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitAtlases() {
  RegoAtlas("game-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitLabels() {
  auto soff = f::CstVal<c::Integer>("S_OFF")->getValue();
  auto tile = f::CstVal<c::Integer>("TILE")->getValue();
  auto wb = cx::VisBox();

  m_scoreLabel = cx::CreateBmfLabel(0,0,"font.SmallTypeWriting", "0");
  m_scoreLabel->setAnchorPoint(cx::AnchorBR());
  m_scoreLabel->setScale(XCFGS()->GetScale());
  m_scoreLabel->setPosition(wb.right - tile - soff,
    wb.top - tile - soff - cx::GetScaledHeight(m_scoreLabel));

  this->addChild(m_scoreLabel, lastZix, ++lastTag);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitIcons() {
  auto soff = f::CstVal<c::Integer>("S_OFF")->getValue();
  auto tile = f::CstVal<c::Integer>("TILE")->getValue();
  auto wb = cx::VisBox();

  lives = f::XLives::Create( this, "health.png", tile + soff,
    wb.top - tile - soff);
}




NS_END(invaders)



