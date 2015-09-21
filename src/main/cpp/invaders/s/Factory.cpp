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
#include "core/CCSX.h"
#include "core/XPool.h"
#include "x2d/MainGame.h"
#include "ash/Engine.h"
#include "n/gnodes.h"
#include "Factory.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Factory::Factory(a::Engine* e, c::Dictionary* options)
  : engine(e)
  , state(options) {

  state->retain();
}

//////////////////////////////////////////////////////////////////////////
//
Factory::~Factory() {
  state->release();
}

//////////////////////////////////////////////////////////////////////////
//
Factory::Factory()
  : engine(nullptr)
  , state(nullptr) {

}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateMissiles(int count) {
  auto p= f::XConfig::GetInstance()->GetPool("missiles");
  auto cb= [=](f::XPool* pp) -> f::ComObj* {
    auto sp = cx::CreateSprite("missile.png");
    sp->setVisible(false);
    f::MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Missile(sp);
  };
  p->Preset(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateExplosions(int count) {
  auto p= f::XConfig::GetInstance()->GetPool("explosions");
  auto cb= [=](f::XPool* pp) -> f::ComObj* {
    auto sp = cx::CreateSprite("boom_0.png");
    sp->setVisible(false);
    f::MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Explosion(sp);
  };
  p->Preset(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateBombs(int count) {
  auto p= f::XConfig::GetInstance()->GetPool("bombs");
  auto cb= [=](f::XPool* pp) -> f::ComObj* {
    auto sp = cx::CreateSprite("bomb.png");
    sp->setVisible(false);
    f::MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Bomb(sp);
  };
  p->Preset(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
const c::Size Factory::CalcImgSize(const stdstr& img) {
  return cx::CalcSize(img);
}

//////////////////////////////////////////////////////////////////////////
//
c::Dictionary* Factory::GetRankInfo(int r) {
  c::Size z= cx::CalcSize("purple_bug_0.png");
  stdstr s0 = "purple_bug_0.png";
  stdstr s1= "purple_bug_1.png";
  int v= 30;
  auto d = c::Dictionary::create();

  if (r < 3) {
    v= 100;
    s0 = "blue_bug_0.png";
    s1 = "blue_bug_1.png";
    z= CalcImgSize("blue_bug_0.png");
  }

  if (r < 5) {
    v= 50;
    s0 = "green_bug_0.png";
    s1= "green_bug_1.png";
    z = CalcImgSize("green_bug_0.png");
  }

  d->setObject(f::Size2::create(z.width, z.height), "size");
  d->setObject(c::Integer::create(v), "value");
  d->setObject(c::String::create(s0), "img0");
  d->setObject(c::String::create(s1), "img1");

  return d;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::FillSquad(f::XPool* pool) {

  int cells = CstVal<cc::Integer>("CELLS")->getValue();
  int cols = CstVal<cc::Integer>("COLS")->getValue();
  auto cfg = f::XConfig::GetInstance();
  auto info= GetRankInfo(0);
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();

  c::Sprite* aa;
  c::Size az;
  int v, row;
  float x,y;

  f::Size2* s2 = DictVal<f::Size2>(info, "size");
  az= s2->getValue();
  row=0;
  for (int n=0; n < cells; ++n) {
    if (n % cols == 0) {
      y = (n == 0) ? wb.top * 0.9
                   : y - az.height - wz.size.height * 4/480;
      x = wb.left + (8/320 * wz.size.width) + HWZ(az);
      row += 1;
      info= GetRankInfo(row);
      s2= DictVal<f::Size2>(info, "size");
      az= s2->getValue();
    }
    c::String* s = DictVal<c::String>(info, "img0");
    aa = s->getCString();
    aa->setPosition(x + HWZ(az), y - HHZ(az));

    c::Vector<c::SpriteFrame*> animFrames(2);
    s=DictVal<c::String>(info, "img0");
    auto f1 = s->getCString();
    s=DictVal<c::String>(info, "img1");
    auto f2 = s->getCString();
    animFrames.pushBack( cx::GetSpriteFrame(f1));
    animFrames.pushBack( cx::GetSpriteFrame(f2));

    aa->runAction(c::RepeatForever::create(
      c::Animate::create(
        c::Animation::createWithSpriteFrames( animFrames, 1))));

    f::MainGame::Get()->AddAtlasItem("game-pics", aa);
    x += az.width + (8/320 * wz.size.width);
    c::Integer* nn= DictVal<c::Integer>(info, "value");
    v= nn->getValue();
    auto co= new Alien(aa, v, row);
    co->status=true;
    pool->Checkin(co);
  }
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* Factory::CreateAliens() {
  auto stepx= DictVal<f::Size2>(state, "alienSize")->getValue().width /3;
  auto ent= engine->CreateEntity("baddies");
  auto cfg = f::XConfig::GetInstance();
  auto p = cfg->GetPool("aliens");

  FillSquad(p);

  ent->Rego(new AlienSquad(p, stepx));
  ent->Rego(new Looper(2));

  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::BornShip() {
  auto s= f::MainGame::Get()->GetPlayer();

  if (NNP(s)) {
    s->Inflate();
  }
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* Factory::CreateShip() {

  auto sz = DictVal<f::Size2>(state, "shipSize")->getValue();
  auto ent= engine->CreateEntity("goodies");
  auto s= cx::CreateSprite("ship_1.png");
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();
  auto y = sz.height + wb.bottom + (5/60 * wz.size.height);
  auto x = wb.left + wz.size.width * 0.5;
  Ship* ship;

  f::MainGame::Get()->AddAtlasItem("game-pics", s);

  ship = new Ship(s, "ship_1.png", "ship_0.png");
  ship->Inflate(x,y);

  ent->Rego(new Velocity(150,0));
  ent->Rego(new Looper(1));
  ent->Rego(new Cannon());
  ent->Rego(new Motion());

  ent->Rego(ship);

  return ent;
}



NS_END(invaders)


