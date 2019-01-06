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
#include "CJKOS-GBK-Font.h"
#include "CJKOS-GBK-FontDlg.h"
#include "CJKOSGBKFOntWriter.h"
#include "fontdotscollector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int FONT_SIZE[]={12,16,24};

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCJKOSGBKFontDlg 对话框

CCJKOSGBKFontDlg::CCJKOSGBKFontDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCJKOSGBKFontDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_font_size = 1;
}

void CCJKOSGBKFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio (pDX,IDC_RADIO12,m_font_size);
	//we subclassed IDC_FONT,so have to comment the following codes
	//DDX_Control(pDX, IDC_FONT, m_font_box);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}

BEGIN_MESSAGE_MAP(CCJKOSGBKFontDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnBnClickedButtonAbout)
	ON_CBN_SELCHANGE(IDC_FONT, OnCbnSelchangeFont)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CCJKOSGBKFontDlg 消息处理程序

BOOL CCJKOSGBKFontDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//额外的初始化代码
	m_font_box.SubclassDlgItem(IDC_FONT,this);
	m_font_box.PostMessage (WM_INITFONTS,0,0);

	m_font_box.SetCurSel(-1);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CCJKOSGBKFontDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCJKOSGBKFontDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCJKOSGBKFontDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCJKOSGBKFontDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CCJKOSGBKFontDlg::OnCbnSelchangeFont()
{
	m_font_box.GetWindowText(m_font_name);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(m_font_name.GetLength()>0);
}

void CCJKOSGBKFontDlg::EnableButtons(BOOL bEnable)
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bEnable);
	GetDlgItem(IDOK)       ->EnableWindow(bEnable);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND|(bEnable?MF_ENABLED:MF_GRAYED));
}

class UI_Update_Helper{
public:
	static CProgressCtrl* m_progress_ctrl;
	static void Init(CProgressCtrl& progress){
		m_progress_ctrl=&progress;
	}
	static void SetRange(int char_count){
		TRACE(_T("total char:%d\n"),char_count);
		m_progress_ctrl->SetRange32(0,char_count);
		m_progress_ctrl->SetPos(0);
	};
	static void OnChar(int repeat_count){		
#ifdef _DEBUG
		static int count=0;
		count+=repeat_count;
		TRACE(_T("font No.%d\n"),count);
#endif
		m_progress_ctrl->OffsetPos(repeat_count);
		//AfxGetApp()->PumpMessage();
	}
};

CProgressCtrl* UI_Update_Helper::m_progress_ctrl=NULL;

typedef CCJKOSGBKFontWriter<CFontDotsCollector,UI_Update_Helper> WRITER;

void CCJKOSGBKFontDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	CString fname;
	fname.Format(_T("ChineseGBK-%s-%02dx%02d.pdb"),m_font_name,FONT_SIZE[m_font_size],FONT_SIZE[m_font_size]);
	CFileDialog dlg(FALSE,_T(".pdb"),fname,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("Palm文件 (*.pdb;*.prc)|*.xlc; *.xls|所有文件 (*.*)|*.*||"),
					this);
	if(dlg.DoModal()==IDOK){
	
		UI_Update_Helper::Init(m_progress);

#if 1
		WRITER font_writer(FONT_SIZE[m_font_size],m_font_name);
		UI_Update_Helper::SetRange(font_writer.GetCharsCount());
		font_writer.WriteCJKOSGBKFontFile(dlg.GetPathName());
#else
		WRITER* font_writer=new WRITER(FONT_SIZE[m_font_size],m_font_name);
		UI_Update_Helper::SetRange(font_writer->GetCharsCount());
		font_writer->WriteCJKOSGBKFontFile(_T("d:\\temp\\test.pdb"));
#endif
	}
}
