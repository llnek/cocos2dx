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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "Resolve.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  BoardNode b;
  ArenaNode a;
  arenaNode = engine->getNodeList(a.typeId());
  boardNode = engine->getNodeList(b.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    syncUp();
    doIt(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::syncUp() {
  auto view= CC_GNLF(PlayView, boardNode, "view");
  auto grid= CC_GNLF(Grid, boardNode, "grid");

  auto nil= CC_CSV(c::Integer, "CV_Z");

  for (int i=0; i < grid->values.size(); ++i) {
    auto v= grid->values[i];
    c::Vec2 c;
    if (v != nil &&
        xrefCell(i, view, c)) {
      auto &z= view->cells[i];
      if (NNP(z)) {
        z->setUserObject(nullptr);
        z->removeFromParent();
      }
      z = drawSymbol(view, c, v, false);
      z->setUserObject(CC_INT(v));
      z->setPosition(c);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::xrefCell(int pos, PlayView *view, c::Vec2 &cell) {
  auto cells = view->boxes.size();
  auto found=false;
  auto delta=0;
  if (pos >= 0 && pos < cells) {
    auto &gg = view->boxes[pos];
    auto x = gg.left + (gg.right - gg.left  - delta) * 0.5f;
    auto y = gg.top - (gg.top - gg.bottom - delta ) * 0.5f;
    // the cell's center
    cell.x=x;
    cell.y=y;
    found=true;
  }
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::doIt(float dt) {

  auto ps = CC_GNLF(Players, boardNode, "players");
  auto gd = CC_GNLF(Grid, boardNode, "grid");
  auto winner= -1;
  ArrDim combo;

  for (int i=0; i < ps->parr.size(); ++i) {
    if (i > 0 &&
        checkWin(ps->parr[i], gd, combo)) {
      winner=i;
      break;
    }
  }

  if (winner > 0) {
    doWin(ps->parr[winner], combo);
  }
  else
  if (checkDraw(gd)) {
    doDraw();
  }
  else {
    auto &q= MGMS()->msgQueue();
    if (q.size() > 0) {
      auto s= q.front();
      q.pop();
      if ("forfeit" == s) {
        doForfeit();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doWin(Player &winner, const ArrDim &combo) {

  auto msg= j::json({
    {"pnum", winner.pnum },
    {"score", 1}
  });

  SENDMSGEX("/hud/score/update", &msg);
  showWinningIcons(combo);
  doDone(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doDraw() {
  auto ps= CC_GNLF(Players, boardNode, "players");
  ps->parr[0].pnum= 0;
  doDone(ps->parr[0]);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doForfeit() {
  auto view = CC_GNLF(PlayView, boardNode, "view");
  auto ps= CC_GNLF(Players, boardNode, "players");
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  auto cur=ss->pnum;
  auto other= cur == 1 ? 2 : cur == 2 ? 1 : 0;

  auto &loser = ps->parr[cur];
  auto &win= ps->parr[other];

  auto msg = j::json({
    {"pnum", win.pnum },
    {"score", 1}
  });

  SENDMSGEX("/hud/score/update", &msg);

  //gray out the losing icons
  F__LOOP(it, view->cells) {
    auto &z = *it;
    if (NNP(z)) {
      auto n = (c::Integer*) z->getUserObject();
      if (n->getValue() == loser.value) {
        auto p= z->getPosition();
        z->removeFromParent();
        z = drawSymbol(view, p, n->getValue(), true);
      }
    }
  }

  doDone(win);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::showWinningIcons(const ArrDim &combo) {

  auto view = CC_GNLF(PlayView, boardNode, "view");
  auto nil = CC_CSV(c::Integer, "CV_Z");

  for (int i=0; i < view->cells.size(); ++i) {
    if (! (s::find(s::begin(combo), s::end(combo), i)
          != combo.end())) {
      //flip the other cells to gray
      auto &z= view->cells[i];
      if (NNP(z) ) {
        auto n= (c::Integer*) z->getUserObject();
        auto p= z->getPosition();
        if (n->getValue() != nil) {
          z->removeFromParent();
          z= drawSymbol(view, p, n->getValue(), true);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doDone(Player &pobj) {

  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  int pnum = pobj.pnum > 0 ? pobj.pnum : 0;

  ss->lastWinner= pnum;

  auto msg= j::json({
    {"winner", pnum  }
  });

  SENDMSG("/hud/timer/hide");
  SENDMSGEX("/hud/end", &msg);

  cx::sfxPlay("game_end");
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkDraw(Grid *gd) {

  auto nil= CC_CSV(c::Integer, "CV_Z");

  return ! (s::find(s::begin(gd->values), s::end(gd->values), nil)
    != gd->values.end());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkWin(Player &p, Grid *game, ArrDim &combo) {

  CCLOG("checking win for %s", p.color.c_str());

  F__LOOP(it, game->GOALS) {

    auto &g = *it;
    int cnt=0;

    for (int i=0; i < g.size(); ++i) {
      auto pos = g[i];
      if (game->values[pos] == p.value) {
        ++cnt;
      }
    }
    if (cnt == g.size()) {
       // found a winning combo, this guy wins!
      s::copy(s::begin(g), s::end(g), s::begin(combo));
      return true;
    }
  }
  return false;
}



NS_END(tttoe)


