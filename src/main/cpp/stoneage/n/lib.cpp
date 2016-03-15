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
#include "core/CCSX.h"
#include "lib.h"
#include "C.h"
#include "Gem.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
static int GEMS_TOTAL= GRID_SIZE_X * GRID_SIZE_Y;
static int TILE_GRID= TILE_SIZE+GRID_SPACE;

static s_arr<sstr,GEMSET_SIZE> SKINS = {
  "gem_blue.png",
  "gem_yellow.png",
  "gem_red.png",
  "gem_green.png",
  "gem_pink.png",
  "gem_white.png"
};

//////////////////////////////////////////////////////////////////////////////
//
static bool find (const f::Cell2I &np, const s_vec<f::Cell2I> &arr) {
  F__LOOP(it, arr) {
    auto &a= *it;
    if (a.x == np.x &&
        a.y == np.y) return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
static void addMatches(GVars *ss, const s_vec<f::Cell2I> &matches) {
  F__LOOP(it, matches) {
    auto &m= *it;
    if (! find(m, ss->matchArray)) {
      ss->matchArray.push_back(m);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
static void checkTypeMatch(GVars *ss, int c, int r) {

  auto type = ss->grid[c]->get(r);
  s_vec<f::Cell2I> temp;
  auto stepC = c;
  auto stepR = r;

  //check top
  while (stepR-1 >= 0 &&
    ss->grid[c]->get(stepR-1) == type) {
    --stepR;
    temp.push_back(f::Cell2I(c, stepR));
  }

  if (temp.size() >= 2) { addMatches(ss, temp); }

  //check bottom
  temp.clear();
  stepR = r;
  while (stepR+1 < GRID_SIZE_Y &&
         ss->grid[c]->get(stepR+1) == type) {
    ++stepR;
    temp.push_back(f::Cell2I(c, stepR));
  }

  if (temp.size() >= 2) { addMatches(ss, temp); }

  //check left
  temp.clear();
  while (stepC-1 >= 0 &&
         ss->grid[stepC-1]->get(r) == type) {
    --stepC;
    temp.push_back(f::Cell2I(stepC, r));
  }

  if (temp.size() >= 2) { addMatches(ss, temp); }

  //check right
  temp.clear();
  stepC = c;
  while (stepC+1 < GRID_SIZE_X &&
         ss->grid[stepC+1]->get(r) == type) {
    ++stepC;
    temp.push_back(f::Cell2I(stepC,  r));
  }

  if (temp.size() >= 2) { addMatches(ss, temp); }
}

//////////////////////////////////////////////////////////////////////////////
//
bool isValidTarget(GVars *ss, int px, int py, const c::Vec2 &touch) {

  auto offbounds = false;
  auto rc=true;

  if (px > ss->selectedIndex.x + 1 ||
      px < ss->selectedIndex.x - 1 ||
      py > ss->selectedIndex.y + 1 ||
      py < ss->selectedIndex.y - 1) {
  offbounds = true; }

  auto cell = sin(atan2(pow(ss->selectedIndex.x - px, 2),
        pow(ss->selectedIndex.y- py, 2)));

  if (cell != 0 && cell != 1) {
    offbounds = true;
  }

  if (offbounds ) {
  return false; }

  auto touchedGem = ss->gemsColMap[px]->get(py);
  if (touchedGem == ss->selectedGem ||
      (px == ss->selectedIndex.x &&
       py == ss->selectedIndex.y)) {
    ss->targetGem = CC_NIL;
    rc=false;
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
GemInfo findGemAtPos(GVars *ss, const c::Vec2 &pos) {

  auto mx = pos.x - ss->gemsContainer->getPositionX();
  auto my = pos.y - ss->gemsContainer->getPositionY();
  auto gridHeight = GRID_SIZE_Y * TILE_GRID;
  auto gridWidth = GRID_SIZE_X * TILE_GRID;

  if (mx < 0) mx = 0;
  if (my < 0) my = 0;

  if (my > gridHeight) my = gridHeight;
  if (mx > gridWidth) mx = gridWidth;

  auto x = ceil((mx - HTV(TILE_SIZE)) / TILE_GRID);
  auto y = ceil((my - HTV(TILE_SIZE)) / TILE_GRID);

  if (x < 1) x = 1;
  if (y < 1) y = 1;
  if (x > GRID_SIZE_X) x = GRID_SIZE_X;
  if (y > GRID_SIZE_Y) y = GRID_SIZE_Y;

  // coz arrays are zero based
  --x;
  --y;
  return GemInfo(x, y , ss->gemsColMap[x]->get(y));
}

//////////////////////////////////////////////////////////////////////////////
//
void selectStartGem(GVars *ss, const GemInfo &touched) {
  if (!ss->selectedGem) {
    ss->selectedGem = touched.gem;
    ss->targetIndex = f::Cell2I();
    ss->targetGem = CC_NIL;
    touched.gem->node->setLocalZOrder(Z_SWAP_2);
    ss->selectedIndex = f::Cell2I(touched.x, touched.y);
    ss->selectedGemPos = touched.gem->pos();
    animateSelected(touched.gem);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void selectTargetGem(GVars *ss, const GemInfo &touched) {
  if (ss->targetGem != CC_NIL) { return; }
  ss->enabled = false;
  ss->targetIndex = f::Cell2I(touched.x, touched.y);
  ss->targetGem = touched.gem;
  ss->targetGem->node->setLocalZOrder(Z_SWAP_1);
  swapGemsToNewPosition(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
bool checkGridMatches(GVars *ss) {

  ss->matchArray.clear();
  auto rc=false;

  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    for (auto r = 0; r < GRID_SIZE_Y; ++r) {
      checkTypeMatch(ss, c, r);
    }
  }

  if (ss->matchArray.size() >= 2) {
    SENDMSG("/game/player/addscore");
    rc= true;
  } else {
    CCLOG("no matches");
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
int getGemType(int idx) {
  assert(idx >= 0 && idx < GEMSET_SIZE);
  return idx;
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr getGemPng(int type) {
  assert(type >= 0 && type < GEMSET_SIZE);
  return SKINS[type];
}

//////////////////////////////////////////////////////////////////////////////
//
int getVertUnique(GVars *ss, int col, int row) {
  assert(col >=0 && col < GRID_SIZE_X);
  assert(row >=0 && row < GRID_SIZE_Y);
  auto t = cx::randInt(GEMSET_SIZE);

  if (col >= 0 && row > 1 &&
      ss->grid[col]->get(row-1) == t &&
      ss->grid[col]->get(row-2) == t) {
      t += 1;
      if (t >= GEMSET_SIZE) { t=0; }
  }
  return t;
}

//////////////////////////////////////////////////////////////////////////////
//
int getVertHorzUnique(GVars *ss, int col, int row) {

  auto t= getVertUnique(ss, col, row);

  if (col > 1 && row >= 0 &&
      ss->grid[col-1]->get(row) == t &&
      ss->grid[col-2]->get(row) == t) {
    auto unique = false;
    while (!unique) {
      t = getVertUnique(ss, col, row);
      if (ss->grid[col-1]->get(row) == t &&
          ss->grid[col-2]->get(row) == t)
      {}
      else {
       unique = true;
      }
    }
  }
  return t;
}

//////////////////////////////////////////////////////////////////////////////
//
void dropSelectedGem(GVars *ss) {
  ss->selectedGem->node->setLocalZOrder(Z_GRID);
  resetSelectedGem(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void onNewSwapComplete(GVars *ss) {

  ss->gemsColMap[ss->selectedIndex.x]->set(
      ss->selectedIndex.y, ss->targetGem);

  ss->gemsColMap[ss->targetIndex.x]->set(
      ss->targetIndex.y, ss->selectedGem);

  ss->grid[ss->selectedIndex.x]->set(
      ss->selectedIndex.y, ss->targetGem->getType());
  ss->grid[ss->targetIndex.x]->set(
      ss->targetIndex.y, ss->selectedGem->getType());

  ss->addingCombos = true;
  ss->combos = 0;

  //check for new matches
  if (checkGridMatches(ss)) {

    //animate matched gems
    if (ss->matchArray.size() > 3) {
      ss->combos += ss->matchArray.size() - 3;
    }

    animateMatches(
        ss,
        ss->matchArray,
        [=]() { collapseGrid(ss); });

    showMatchParticle(ss, ss->matchArray);

    auto msg= j::json({
        {"score", (int) ss->matchArray.size() * POINTS },
        {"type", "match2"}
        });
    SENDMSGEX("/game/player/earnscore", &msg);
    cx::sfxPlay("match2");

  } else {

    //no matches, swap gems back
    swapGems(
        ss,
        ss->targetGem,
        ss->selectedGem,
        [=]() { ss->enabled = true; });

    ss->gemsColMap[ss->selectedIndex.x]->set(
        ss->selectedIndex.y, ss->selectedGem);
    ss->gemsColMap[ss->targetIndex.x]->set(
        ss->targetIndex.y, ss->targetGem);

    ss->grid[ss->selectedIndex.x]->set(
        ss->selectedIndex.y, ss->selectedGem->getType());
    ss->grid[ss->targetIndex.x]->set(
        ss->targetIndex.y, ss->targetGem->getType());

    cx::sfxPlay("wrong");
  }

  ss->selectedGem = CC_NIL;
  ss->targetGem = CC_NIL;
}

//////////////////////////////////////////////////////////////////////////////
//
void swapGemsToNewPosition(GVars *ss) {

  swapGems(
      ss,
      ss->selectedGem,
      ss->targetGem,
           [=]() { onNewSwapComplete(ss); });
}

//////////////////////////////////////////////////////////////////////////////
//
void dbgGemTypes(GVars *ss, int col) {
  auto g= ss->grid[col];
  for (auto n=0; n < g->size(); ++n) {
    CCLOG("row[%d].type = %d", n, g->get(n));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void dbgGems(GVars *ss, int c) {
  auto g= ss->gemsColMap[c];
  for (auto n=0; n < g->size(); ++n) {
    auto c= g->get(n);
    CCLOG("gem[%d].pos= %d, %d",
        n,
        (int)c->node->getPositionX(), (int)c->node->getPositionY());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void onGridCollapseComplete(GVars *ss) {

  F__LOOP(it,ss->allGems) {
    auto gem = *it;
    auto pos=gem->pos();
    auto yIndex = ceil((pos.y - HTV(TILE_SIZE))/TILE_GRID);
    auto xIndex = ceil((pos.x - HTV(TILE_SIZE))/TILE_GRID);
    --yIndex;
    --xIndex;// zero based
    ss->gemsColMap[xIndex]->set(yIndex, gem);
    ss->grid[xIndex]->set(yIndex, gem->getType());
  }

  if (checkGridMatches(ss)) {

       //animate matched games
     if (ss->addingCombos) {
       if (ss->matchArray.size() > 3) {
         ss->combos += (ss->matchArray.size() - 3);
       }
     }

     animateMatches(
         ss,
         ss->matchArray,
         [=]() { collapseGrid(ss); });

     showMatchParticle(ss,ss->matchArray);

     auto msg= j::json({
         {"score", (int)ss->matchArray.size() * POINTS },
         {"type", "match"}
         });
     SENDMSGEX("/game/player/earnscore",&msg);

     cx::sfxPlay("match");

  } else {

    //no more matches, check for combos
    if (ss->combos > 0) {
      //now turn random gems into diamonds
      s_vec<f::Cell2I> removeGems;
      s_vec<ecs::Node*> diamonds;
      int i = 0;

      auto p1=MGMS()->getPool("DiamondParticles");
      auto p2=MGMS()->getPool("Diamonds");

      f::randSeed();

      while (i < ss->combos) {
        ++i;
        Gem *randomGem = CC_NIL;
        int randomY = 0;
        int randomX=0;

        while (!randomGem) {
          randomX = cx::randInt(GRID_SIZE_X);
          randomY = cx::randInt(GRID_SIZE_Y);
          randomGem = ss->gemsColMap[randomX]->get(randomY);
          if (randomGem->getType() == TYPE_GEM_WHITE) { randomGem = CC_NIL; }
        }

        auto diamondPart = p1->take(true);
        auto diamond = p2->take(true);
        auto s1=CC_GEC(Particle,diamondPart,"f/CPixie");
        auto s2=CC_GEC(Diamond,diamond,"f/CPixie");
        auto pos= randomGem->pos();

        s1->setPos(pos.x, pos.y);
        s2->setPos(pos.x,pos.y);

        removeGems.push_back(f::Cell2I(randomX, randomY));
        diamonds.push_back((ecs::Node*)diamond);
      }

      auto msg= j::json({
          {"score", (int)diamonds.size() * DIAMOND_POINTS },
          {"type", "diamond"}
      });
      SENDMSGEX("/game/player/earnscore", &msg);

      animateMatches(
          ss,
          removeGems,
          [=]() { collapseGrid(ss); });

      collectDiamonds( diamonds);
      ss->combos = 0;
      cx::sfxPlay("diamond");

    } else {

      ss->enabled = true;
    }

    ss->addingCombos = false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void collapseGrid(GVars *ss) {

  F__LOOP(it, ss->matchArray) {
    auto &z= *it;
    ss->grid[z.x]->set(z.y, -1);
  }

  F__LOOP(it,ss->grid) {
    s_vec<int> nc;
    auto gc= *it;
    int v;
    for (auto i=0; i < gc->size(); ++i) {
      v=gc->get(i);
      if (v != -1) {
        nc.push_back(v);
      }
    }

    for (auto i=0; i < nc.size(); ++i) {
      gc->set(i, nc[i]);
    }
    for (auto i= nc.size();  i < gc->size(); ++i) {
      gc->set(i,-1);
    }
  }

  animateCollapse(ss,
      [=]() { onGridCollapseComplete(ss); });
}

//////////////////////////////////////////////////////////////////////////////
//
int getNewGem() {
  return cx::randInt(GEMSET_SIZE);
}

//////////////////////////////////////////////////////////////////////////////
//
void showMatchParticle(GVars *ss, const s_vec<f::Cell2I> &matches) {
  auto p= MGMS()->getPool("MatchParticles");
  F__LOOP(it,matches) {
    auto &pos= *it;
    auto gem = ss->gemsColMap[pos.x]->get(pos.y);
    auto part= p->take(true);
    auto ps= CC_GEC(f::CPixie,part,"f/CPixie");
    ps->setPos(
        gem->node->getPositionX() + ss->gemsContainer->getPositionX(),
        gem->node->getPositionY() + ss->gemsContainer->getPositionY());
  }
}

static int animatedGems=0;

//////////////////////////////////////////////////////////////////////////////
//
void animateIntro(GVars *ss) {
  ss->enabled = false;
  for (auto i = 0; i < GRID_SIZE_X; ++i) {
    dropColumn(ss, i);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void dropColumn(GVars *ss, int col) {

  auto onAnimatedColumn= [=]() {
    animatedGems += 1;
    if (animatedGems == GEMS_TOTAL) {
      ss->enabled = true;
      SENDMSG("/game/start/timer");
    }
  };

  float delay = col>0 ? cx::rand() * 1.5f : 0;
  auto m= ss->gemsColMap[col];
  Gem *gem;
  for (auto i=0; i < m->size(); ++i) {
    gem = m->get(i);
    gem->show();
    auto finalY = gem->node->getPositionY();
    gem->node->setPositionY(finalY + 800);
    gem->node->runAction(
      c::Sequence::create(
        c::DelayTime::create(delay),
        c::EaseBounceOut::create(
          c::MoveTo::create(
            2,
            c::Vec2(gem->node->getPositionX(), finalY))),
        c::CallFunc::create(onAnimatedColumn),
        CC_NIL));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void animateSelected(Gem *gem) {
  gem->select();
  gem->node->stopAllActions();
  gem->node->runAction(
    c::EaseBounceOut::create(
      c::RotateBy::create(0.5, 360)));
}

//////////////////////////////////////////////////////////////////////////////
//
void resetSelectedGem(GVars *ss) {
  auto gemPosition = ss->selectedGemPos;
  auto gem = ss->selectedGem;
  gem->node->runAction(
      c::EaseElasticOut::create(
        c::MoveTo::create(0.25, gemPosition)));
}

//////////////////////////////////////////////////////////////////////////////
//
void swapGems(GVars *ss, Gem *gemOrigin, Gem *gemTarget, VOIDFN onComplete) {

  auto origin = ss->selectedGemPos;
  auto target = gemTarget->pos();

  auto moveSelected = c::EaseBackOut::create(
      c::MoveTo::create(0.8, target));
  auto moveTarget = c::EaseBackOut::create(
      c::MoveTo::create(0.8, origin) );

  gemOrigin->deselect();
  gemOrigin->node->runAction(moveSelected);
  gemTarget->node->runAction(
      c::Sequence::create(
        moveTarget,
        c::CallFunc::create(onComplete),
        CC_NIL));
}

static int animatedMatchedGems = 0;

//////////////////////////////////////////////////////////////////////////////
//
void animateMatches(
    GVars *ss,
    const s_vec<f::Cell2I> &matches, VOIDFN onComplete) {

  animatedMatchedGems = matches.size();

  auto onCompleteMe=[=]() {
    animatedMatchedGems -= 1;
    if (animatedMatchedGems == 0) {
      onComplete();
    }
  };

  F__LOOP(it,matches) {
    auto &pt= *it;
    auto gem = ss->gemsColMap[pt.x]->get(pt.y);
    gem->node->stopAllActions();
    gem->node->runAction(
        c::Sequence::create(
          c::EaseBackOut::create(
            c::ScaleTo::create(0.3, 0)),
          c::CallFunc::create(onCompleteMe),
          CC_NIL));
  }
}

static int animatedCollapsedGems = 0;

//////////////////////////////////////////////////////////////////////////////
//
void animateCollapse(GVars *ss, VOIDFN onComplete) {

  animatedCollapsedGems = 0;

  f::randSeed();

  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    auto cm = ss->gemsColMap[c];
    int drop = 1;
    for (auto r = 0; r < cm->size(); ++r) {
        auto gem = cm->get(r);
      //if this gem has been resized, move it to the top
      if (gem->node->getScaleX() != 1) {
        gem->node->setPositionY( (GRID_SIZE_Y+drop) * TILE_GRID);
        animatedCollapsedGems += 1;
        gem->setType(getNewGem());
        gem->show();
        auto newY = (GRID_SIZE_Y - (drop - 1)) * TILE_GRID;
        dropGemTo(gem, newY, 0.2, onComplete);
        drop += 1;
      } else {
        if (drop > 1) {
          animatedCollapsedGems += 1;
          auto newY = gem->node->getPositionY() - (drop-1) * TILE_GRID;
          dropGemTo(gem, newY, 0, onComplete);
        }
      }
    }

  }
}

//////////////////////////////////////////////////////////////////////////////
//
void dropGemTo(Gem *gem, float y, float delay, VOIDFN onComplete)  {

  gem->node->stopAllActions();
  gem->reset();

  auto onCompleteMe=[=]() {
    animatedCollapsedGems -= 1;
    if (animatedCollapsedGems == 0) {
      onComplete();
    }
  };

  auto action = c::Sequence::create(
      c::DelayTime::create(delay),
      c::EaseBounceOut::create(
        c::MoveTo::create(
          0.6, c::Vec2(gem->node->getPositionX(), y))),
      c::CallFunc::create(onCompleteMe),
      CC_NIL);

  gem->node->runAction(action);
}

//////////////////////////////////////////////////////////////////////////////
//
void collectDiamonds(const s_vec<ecs::Node*> &diamonds) {

  auto removeDiamond=[=](c::Node *n) {
    CC_HIDE(n);
  };
  auto wb=cx::visBox();
  int i=1;

  F__LOOP(it,diamonds) {
    auto d= *it;
    auto action = c::Sequence::create(
        c::DelayTime::create(i * 0.05),
        c::EaseBackIn::create(
          c::MoveTo::create(
            0.8, c::Vec2(50, wb.top - 50))),
        c::CallFuncN::create(removeDiamond),
        CC_NIL);
    auto sp=CC_GEC(f::CPixie,d,"f/CPixie");
    sp->node->runAction(action);
    ++i;
  }
}


NS_END






