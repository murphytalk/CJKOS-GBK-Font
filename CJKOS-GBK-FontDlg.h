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
#include "d:\devtools\msvs8\vc\atlmfc\include\afxwin.h"
#include "FontComboBox.h"
#include "d:\devtools\msvs8\vc\atlmfc\include\afxcmn.h"

// CCJKOSGBKFontDlg 对话框
class CCJKOSGBKFontDlg : public CDialog
{
// 构造
public:
	CCJKOSGBKFontDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CJKOSGBKFONT_DIALOG };
	void EnableButtons(BOOL bEnable);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
public:
	CString m_font_name;
	CFontComboBox m_font_box;
	int m_font_size;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnCbnSelchangeFont();
	afx_msg void OnBnClickedButton1();
	CProgressCtrl m_progress;
};
