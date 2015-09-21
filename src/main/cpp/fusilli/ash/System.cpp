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

#include "System.h"
#include "Engine.h"
NS_BEGIN(ash)


//////////////////////////////////////////////////////////////////////////////
//
System::System(int p) : System() {
  this->priority= p;
}

//////////////////////////////////////////////////////////////////////////////
//
System::~System() {
}

//////////////////////////////////////////////////////////////////////////////
//
System::System()
  : previous(nullptr)
  , next(nullptr)
  , active(false)
  , priority(0) {

}

//////////////////////////////////////////////////////////////////////////////
//
void System::Restart() {
  active=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void System::Suspend() {
  active=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void System::AddToEngine(Engine* e) {
}

//////////////////////////////////////////////////////////////////////////////
//
void System::RemoveFromEngine(Engine* e) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool System::Update(float time) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool System::Is(const SystemType& type) {
  return type == TypeId();
}




NS_END(ash)


