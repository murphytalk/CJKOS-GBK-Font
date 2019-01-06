//*************************************************************************
//CFontComboBox.h

#if !defined(AFX_CUSTCOMBOBOX_H__F8528B4F_396E_11D1_9384_00A0248F6145__INCLUDED_)
#define AFX_CUSTCOMBOBOX_H__F8528B4F_396E_11D1_9384_00A0248F6145__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FontComboBox.h : header file
//
///////////////////////////////////////////////////////////////////////////
//
// CFontComboBox window

typedef enum {FONTS} STYLE;      //Why have I enumerated, Cos, Maybe I might want something other than Fonts here
#define WM_INITFONTS          (WM_USER + 123)
//I chose 123 cos nobody might use the same exact number.. I can improve this by use RegisterWindowMessage..


class CFontComboBox : public CComboBox
{
// Construction
public:
     CFontComboBox();
     CFontComboBox (STYLE);

// Attributes
public:
     void SetHilightColors (COLORREF hilight,COLORREF hilightText)
     {
          m_clrHilight = hilight;
          m_clrHilightText = hilightText;
     };
     void SetNormalColors (COLORREF clrBkgnd,COLORREF clrText)
     {
          m_clrNormalText = clrText;
          m_clrBkgnd = clrBkgnd;
     };
     static BOOL CALLBACK EnumFontProc (LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData);
     //static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe,NEWTEXTMETRICEX *lpntme,DWORD FontType,LPARAM lParam);
     void FillFonts ();
     int  GetSelFont (LOGFONT&);


// Operations
public:

// Overrides
     // ClassWizard generated virtual function overrides
     //{{AFX_VIRTUAL(CFontComboBox)
     public:
     virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
     virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
     protected:
     virtual void PreSubclassWindow();
     //}}AFX_VIRTUAL

// Implementation
public:
     virtual ~CFontComboBox();

     // Generated message map functions
protected:
     STYLE m_enStyle;
     COLORREF m_clrHilight;
     COLORREF m_clrNormalText;
     COLORREF m_clrHilightText;
     COLORREF m_clrBkgnd;
     BOOL m_bInitOver;


     void DrawDefault (LPDRAWITEMSTRUCT);
     void DrawFont(LPDRAWITEMSTRUCT);

     void InitFonts ();
     //{{AFX_MSG(CFontComboBox)
     afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
     afx_msg void OnDestroy();
     //}}AFX_MSG
     afx_msg   long OnInitFonts (WPARAM, LPARAM);
     DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////
//

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif //!defined(AFX_CUSTCOMBOBOX_H__F8528B4F_396E_11D1_9384_00A0248F6145__INCLUDED_)



