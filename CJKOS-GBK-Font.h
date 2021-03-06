/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphTalk(AKA:DeepWater@Hi-PDA)'s CJKOS GBK Font Creator,
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**
** murphytalk@gmail.com
***********************************************************************/

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号


// CCJKOSGBKFontApp:
// 有关此类的实现，请参阅 CJKOS-GBK-Font.cpp
//

class CCJKOSGBKFontApp : public CWinApp
{
public:
	CCJKOSGBKFontApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCJKOSGBKFontApp theApp;
