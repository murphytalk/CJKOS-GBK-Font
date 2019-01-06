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
#include "StdAfx.h"
#include "fontdotscollector.h"
#include "gbkmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFontDotsCollector *CFontDotsCollector::ms_pThis=NULL;
CString CFontDotsCollector::ms_wnd_classname;
CString CFontDotsCollector::ms_wndclass;

void CFontDotsCollector::CDotWnd::DrawFont(CString& unicode)
{
	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(&m_font);
	CRect rc;
	GetClientRect(rc);
	FillRect(dc.m_hDC,rc,(HBRUSH)::GetStockObject(WHITE_BRUSH));
	dc.TextOut(0,0,unicode);
	dc.SelectObject(pOldFont);
}

#if 0
void CFontDotsCollector::CDotWnd::OnPaint()
{
	CPaintDC dc(this);
	dc.TextOut(0,0,m_txt);
}
#endif

CFontDotsCollector::CFontDotsCollector(int font_size,LPCTSTR fontname):
	m_font_size(font_size)
{
	if(ms_wndclass.GetLength()==0){
		ms_wndclass=AfxRegisterWndClass(0,0,(HBRUSH)::GetStockObject(WHITE_BRUSH),0);
	}

	//create the hidden window which is used to get font dots
#if 0
	DWORD style=WS_POPUP;
#ifdef _DEBUG
	style|=WS_VISIBLE;
#endif
#else
	DWORD style=WS_POPUP|WS_VISIBLE;
#endif
	m_wnd.CreateEx(0,ms_wndclass,NULL,style,CRect(0,0,font_size,font_size),NULL,0);
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	//lf.lfWeight=FW_THIN;
	//lf.lfQuality=PROOF_QUALITY;//NONANTIALIASED_QUALITY;
	//lf.lfCharSet=
	lf.lfHeight = -font_size;
	CClientDC dc(&m_wnd);
	lf.lfHeight =font_size;//-MulDiv(font_size, GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72);
	wcscpy(lf.lfFaceName,fontname);
	VERIFY(m_wnd.m_font.CreateFontIndirect(&lf));

	//m_pOldFont = dc.SelectObject(&m_font);

}

CFontDotsCollector::~CFontDotsCollector(void)
{
	CClientDC dc(&m_wnd);
	//dc.SelectObject(m_pOldFont);
	m_wnd.m_font.DeleteObject();

	m_wnd.DestroyWindow();
}

void CFontDotsCollector::Begin(int font_size,LPCTSTR fontname)
{
	delete ms_pThis;

	ms_pThis=new CFontDotsCollector(font_size,fontname);
}

void CFontDotsCollector::End()
{
	delete ms_pThis;
	ms_pThis=NULL;
}

void CFontDotsCollector::GetFontDots(CString& unicode/*unicode码*/,
									 int font_size/*字体点阵数*/,
									 char* font_dots/*点阵接收缓冲区*/,
									 int buf_size/*缓冲区大小*/)
{
	//draw char
	CDotWnd& wnd=ms_pThis->m_wnd;

	wnd.DrawFont(unicode);
	
	CClientDC dc(&wnd);

	////get dots
	//clear buffer
	memset(font_dots,0,buf_size);

	CRect rc;
	wnd.GetClientRect(rc);
	//ASSERT(rc.Width()*rc.Height()<=buf_size)
	
	int bits,shift;
	int chr_idx=0; 
	int bit_idx=0; //bit index in one byte

	for(int row=0;row<rc.Height();row++){
		for(int col=0;col<rc.Width();col++){
			COLORREF clr=dc.GetPixel(col,row);
			if(clr!=0xFFFFFF){
				bits=row*font_size+col;
				chr_idx=bits/8;
				bit_idx=bits%8;
				shift  =7-bit_idx;
				font_dots[chr_idx]|=1<<shift;	
			}
		}
	}
}

