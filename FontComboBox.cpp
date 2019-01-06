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
#include "stdafx.h"
#include "FontComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////
//
// CFontComboBox

//Initial values of the text and highlight stuff

CFontComboBox::CFontComboBox()
{
     m_enStyle = FONTS;
     m_clrHilight = GetSysColor (COLOR_HIGHLIGHT);
     m_clrNormalText = GetSysColor (COLOR_WINDOWTEXT);
     m_clrHilightText = GetSysColor (COLOR_HIGHLIGHTTEXT);
     m_clrBkgnd = GetSysColor (COLOR_WINDOW);
     m_bInitOver = FALSE;

}


CFontComboBox::CFontComboBox (STYLE enStyle)
{
     m_enStyle = enStyle;
     m_clrHilight = GetSysColor (COLOR_HIGHLIGHT);
     m_clrNormalText = GetSysColor (COLOR_WINDOWTEXT);
     m_clrHilightText = GetSysColor (COLOR_HIGHLIGHTTEXT);
     m_clrBkgnd = GetSysColor (COLOR_WINDOW);
     m_bInitOver =FALSE;
}

CFontComboBox::~CFontComboBox()
{
}


BEGIN_MESSAGE_MAP(CFontComboBox, CComboBox)
     //{{AFX_MSG_MAP(CFontComboBox)
     ON_WM_CREATE()
     ON_WM_DESTROY()
     //}}AFX_MSG_MAP
     ON_MESSAGE (WM_INITFONTS,OnInitFonts)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
//
// CFontComboBox message handlers


void CFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
     //I might want to add something else someday
     switch (m_enStyle)
     {
     case FONTS:
          DrawFont(lpDrawItemStruct);
          break;
     }
}


//I dont need the MeasureItem to do anything. Whatever the system says, it stays

void CFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
}

void CFontComboBox::DrawFont(LPDRAWITEMSTRUCT lpDIS)
{
     CDC* pDC = CDC::FromHandle(lpDIS->hDC);
     CRect rect;

     TRACE0 ("In Draw Font\n");

    // draw the colored rectangle portion
     rect.CopyRect(&lpDIS->rcItem);

     pDC->SetBkMode( TRANSPARENT );

     if (lpDIS->itemState & ODS_SELECTED)
     {
          pDC->FillSolidRect (rect,m_clrHilight);
          pDC->SetTextColor (m_clrHilightText);
     }
     else
     {
          pDC->FillSolidRect (rect,m_clrBkgnd);
          pDC->SetTextColor (m_clrNormalText);
     }

     if ((int)(lpDIS->itemID) < 0) // Well its negetive so no need to draw text
     {
     }
     else
     {
          CString strText;
          GetLBText (lpDIS->itemID,strText);
          CFont newFont;
          CFont *pOldFont;

          ((LOGFONT*)lpDIS->itemData)->lfHeight = 90; //9 point size
          ((LOGFONT*)lpDIS->itemData)->lfWidth = 0;
          newFont.CreatePointFontIndirect ((LOGFONT*)lpDIS->itemData);
          pOldFont = pDC->SelectObject (&newFont);
	  
	  rect.left+=5;

          pDC->DrawText(strText, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
          pDC->SelectObject (pOldFont);
          newFont.DeleteObject ();
     }
}

void CFontComboBox::InitFonts ()
{
     CDC *pDC = GetDC ();
     ResetContent (); //Delete whatever is there
/*
     LOGFONT log;
     log.lfCharSet=ANSI_CHARSET|CHINESEBIG5_CHARSET|GB2312_CHARSET;
     log.lfFaceName[0]=0;
     EnumFontFamiliesEx(pDC->GetSafeHdc(),&log,(FONTENUMPROC) EnumFontFamExProc,(LPARAM)this,0);//Enumerate
*/
     EnumFonts (pDC->GetSafeHdc(),NULL,(FONTENUMPROC) EnumFontProc,(LPARAM)this);//Enumerate

     m_bInitOver = TRUE;
}
/*
int CALLBACK CFontComboBox::EnumFontFamExProc(ENUMLOGFONTEX *lpelfe,NEWTEXTMETRICEX *lpntme,DWORD FontType,LPARAM lParam)
{
	 //Add only TTF fellows, If you want you can change it to check for others
	if(FontType==TRUETYPE_FONTTYPE){
		int index = ((CFontComboBox *) lParam)->AddString((LPCTSTR)lpelfe->elfFullName);
		LPLOGFONT lpLF = new LOGFONT;
		CopyMemory ((PVOID) lpLF,(CONST VOID *) &(lpelfe->elfLogFont),sizeof (LOGFONT));
		((CFontComboBox *) lParam)->SetItemData (index,(DWORD) lpLF);
	}
	return TRUE;
}
*/
BOOL CALLBACK CFontComboBox::EnumFontProc (LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)
{
	TRACE(_T("font %s,charset is %d\n"),lplf->lfFaceName,lplf->lfCharSet);
	//fixme:why charset of MingLiu is 0(ANSI)?
	//if (dwType == TRUETYPE_FONTTYPE /*&& (lplf->lfCharSet==CHINESEBIG5_CHARSET || lplf->lfCharSet==GB2312_CHARSET)*/)
     {
          int index = ((CFontComboBox *) lpData)->AddString(lplf->lfFaceName);
          LPLOGFONT lpLF;
          lpLF = new LOGFONT;
          CopyMemory ((PVOID) lpLF,(CONST VOID *) lplf,sizeof (LOGFONT));
          ((CFontComboBox *) lpData)->SetItemData (index,(DWORD) lpLF);
     }
     return TRUE;
}

int CFontComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
     if (CComboBox::OnCreate(lpCreateStruct) == -1)
          return -1;

     // TODO: Add your specialized creation code here
     if (m_enStyle == FONTS)
     {
          PostMessage (WM_INITFONTS,0,0);
     }
     return 0;
}

long CFontComboBox::OnInitFonts (WPARAM, LPARAM)
{
     InitFonts ();
     return 0L;
}


void CFontComboBox::OnDestroy()
{

     if (m_enStyle == FONTS)
     {
          int nCount;
          nCount = GetCount ();
          for (int i = 0; i <  nCount; i++)
          {
               delete ((LOGFONT*)GetItemData (i)); //delete the LOGFONTS actually created..
          }
     }
     // TODO: Add your message handler code here
     CComboBox::OnDestroy();
}

void CFontComboBox::FillFonts ()
{
     m_enStyle = FONTS;
     PostMessage (WM_INITFONTS,0,0); //Process in one place
}

int  CFontComboBox::GetSelFont (LOGFONT& lf)
{
     int index = GetCurSel ();
     if (index == LB_ERR)
          return LB_ERR;
     LPLOGFONT lpLF = (LPLOGFONT) GetItemData (index);
     CopyMemory ((PVOID)&lf, (CONST VOID *) lpLF, sizeof (LOGFONT));
     return index; //return the index here.. Maybe the user needs it:-)
}

void CFontComboBox::PreSubclassWindow()
{
     // TODO: Add your specialized code here and/or call the base class
     //Tried to do what Roger Onslow did for the button.. Did not work..?? Any R&D guys around :-)
}
