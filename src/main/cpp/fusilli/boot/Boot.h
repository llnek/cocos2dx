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

#if !defined(__BOOT_H__)
#define __BOOT_H__

#include "platform/CCCommon.h"
#include "core/fusilli.h"
#include "i18n/L10N.h"
NS_USING(cocos2d)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Boot {
private:

  DISALLOW_COPYASSIGN(Boot)
  void PreLaunch();
  void InitAudio();
  L10NCache l10n;

public:

  virtual ~Boot();
  Boot();

};





NS_END(fusilli)
#endif

