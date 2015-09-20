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

#include "Collide.h"
NS_BEGIN(invaders)



//////////////////////////////////////////////////////////////////////////
//
Collide* Collide::Create(Factory* f, c::Dictionary* d) {
  auto s = new Collide();
  s->Set(f,d);
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
Collide::~Collide() {
}

//////////////////////////////////////////////////////////////////////////
//
Collide::Collide()
  : aliens(nullptr)
  , ships(nullptr) {
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::RemoveFromEngine(a::Engine* e) {
  aliens= nullptr;
  ships= nullptr;
  engine=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::AddToEngine(a::Engine* e) {
  aliens= e->GetNodeList(AlienMotionNode::TypeId());
  ships= e->GetNodeList(ShipMotionNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::Update(float dt) {
  auto enemies= aliens->head;
  auto ship = ships->head;

  // 1. get rid of all colliding bombs & missiles.
  //this.checkMissilesBombs();
  // 2. kill aliens
  CheckMissilesAliens(enemies);

  if (NNP(ship)) {
    // 3. ship ok?
    CheckShipBombs(ship);
    // 4. overruned by aliens ?
    if (NNP(enemies)) {
      CheckShipAliens(enemies, ship);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::MaybeCollide(f::ComObj* a, f::ComObj* b) {
  return cx::Collide0(a->sprite, b->sprite);
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckMissilesBombs() {

  auto cfg = f::XConfig::GetInstance();
  auto mss = cfg->GetPool("missiles");
  auto bbs = cfg->GetPool("bombs");
  auto m = mss->Elements();
  auto b= bbs->Elements();

  for (auto it = m.begin(); it != m.end(); ++it) {
    for (auto it2 = b.begin(); it2 != b.end(); ++it2) {
      auto e2 = *it2;
      auto e= *it;
      if (e2->status &&
          e->status &&
          MaybeCollide(m,b)) {
        e2->Hurt();
        e->Hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckMissilesAliens(a::Node* node) {

  AlienSquad* sqad= a::NodeFld<AlienSquad>(node, "aliens");
  auto cfg = f::XConfig::GetInstance();
  auto mss = cfg->GetPool("missiles");
  auto c = sqad->Elements();
  auto c2 = mss->Elements();

  for (auto it = c.begin(); it != c.end(); ++it) {
    for (auto it2 = c2.begin(); it2 != c2.end(); ++it2) {
      auto e2= *it2;
      auto e = *it;
      if (e->status &&
          MaybeCollide(e,e2)) {
        e->Hurt();
        e2->Hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckShipBombs(a::Node* node) {

  Ship* ship= a::NodeFld<Ship>(node, "ship");
  auto cfg = f::XConfig::GetInstance();
  auto bbs= cfg->GetPool("bombs");
  auto c= bbs->Elements();

  if (ship->status)
  for (auto it = c.begin(); it != c.end(); ++it) {
    auto b = *it;
    if (b->status &&
        MaybeCollide(ship, b)) {
      ship->Hurt();
      b->Hurt();
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckShipAliens(a::Node* anode, a::Node* snode) {

  AlienSquad* sqad= a::NodeFld<AlienSquad>(anode, "aliens");
  Ship* ship = a::NodeFld<Ship>(snode, "ship");
  auto c = sqad->Elements();
  auto sz= sqad->Size();

  if (ship->status)
  for (auto it = c.begin(); it != c.end(); ++it) {
    auto a = *it;
    if (a->status &&
        MaybeCollide(ship, a)) {
      ship->Hurt();
      a->Hurt();
    }
  }
}



NS_END(invaders)


