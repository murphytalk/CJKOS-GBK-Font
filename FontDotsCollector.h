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
class CFontDotsCollector
{
	class CDotWnd:public CWnd
	{
	public:
		void DrawFont(CString& unicode);
		CFont *m_pOldFont,m_font;
	};
public:
	CFontDotsCollector(int font_size,LPCTSTR fontname);
	~CFontDotsCollector(void);

	void SetFontSize(int size);
public:
	static CString ms_font_name;
	static CString ms_wnd_classname;
	static CFontDotsCollector *ms_pThis;

	static void Begin(int font_size,LPCTSTR fontname);
	static void End();
	static void GetFontDots(CString& unicode/*unicode码*/,int font_size/*字体点阵数*/,
							char* font_dots/*点阵接收缓冲区*/,int buf_size/*缓冲区大小*/);

private:
	static CString ms_wndclass;

	int m_font_size;
	CDotWnd m_wnd;
};

