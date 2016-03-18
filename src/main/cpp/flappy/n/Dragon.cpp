// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "n/Dragon.h"


NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void Dragon::init() {
  auto wb=cx::visBox();
  // create sprite and add to GameWorld's sprite batch node
  node = cx::reifySprite("dhch_1");
  dragonPosition = c::Vec2(wb.right * 0.2, wb.cy);
  node->setPosition(dragonPosition);
  parentNode->addAtlasItem("dhtex", node, E_LAYER_PLAYER);

  // fetch flying animation from cache & repeat it on the dragon's  sprite
  auto anim= CC_ACAC()->getAnimation("dragonFlying");
  auto a= c::RepeatForever::create(c::Animate::create(anim));
  a->setTag(ANIMATION_ACTION_TAG);
  node->runAction(a);

  // create a hover movement and repeat it on the dragon's sprite
  auto seq = c::Sequence::create(
      c::EaseSineOut::create(
        c::MoveBy::create(anim->getDuration()/2, c::Vec2(0, 10))),
      c::EaseSineOut::create(
        c::MoveBy::create(anim->getDuration()/2, c::Vec2(0, -10))),
      CC_NIL);
  a = c::RepeatForever::create(seq);
  a->setTag(MOVEMENT_ACTION_TAG);
  node->runAction(a);
}

//////////////////////////////////////////////////////////////////////////////
//
void Dragon::onGameStart() {
  // hover should stop once the game has started
  node->stopActionByTag(MOVEMENT_ACTION_TAG);
  // gravity should be applied once the game has started
  mustApplyGravity = true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Dragon::update(float dt) {
  // calculate bounding box after applying gravity
  auto newAABB = CC_BBOX(node);
  newAABB.origin.y= newAABB.origin.y + dragonSpeed.y;

  // check if the dragon has touched the roof of the castle
  if (newAABB.origin.y <= ss->castleRoof) {
    // stop downward movement and set position to the roof of the castle
    dragonSpeed.y = 0;
    dragonPosition.y = ss->castleRoof + HHZ(newAABB.size);

    // dragon must die
    dragonDeath();
  }
  // apply gravity only if game has started
  else if (mustApplyGravity) {
    // clamp gravity to a maximum of MAX_DRAGON_SPEED & add it
    dragonSpeed.y = ((dragonSpeed.y + GRAVITY) < MAX_DRAGON_SPEED ) ? MAX_DRAGON_SPEED : (dragonSpeed.y + GRAVITY);
  }

  // update position
  dragonPosition.y += dragonSpeed.y;
  node->setPosition(dragonPosition);
}

//////////////////////////////////////////////////////////////////////////////
//
void Dragon::dragonFlap() {
  auto wb = cx::visBox();
  // don't flap if dragon will leave the top of the screen
  if (dragonPosition.y + FLAP_FORCE >= wb.top) {
  return; }

  // add flap force to speed
  dragonSpeed.y = FLAP_FORCE;
  cx::sfxPlay("flap");
}

//////////////////////////////////////////////////////////////////////////////
//
void Dragon::dragonDeath() {
  auto wb=cx::visBox();
  // stop the frame based animation...dragon can't fly once its dead
  node->stopAllActions();
  node->runAction(
      c::Sequence::create(
        c::EaseSineOut::create(
            c::MoveBy::create(0.25, c::Vec2(0, node->getContentSize().height))),
        c::EaseSineIn::create(c::MoveTo::create(0.5, c::Vec2(wb.right * 0.2, ss->castleRoof))),
        c::CallFunc::create([=]() {
            SENDMSG("/game/player/lose");
        }),
        CC_NIL));
  cx::sfxPlay("crash");
}



NS_END




