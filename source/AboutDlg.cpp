// AboutDlg.cpp : 实现文件
// SUN Junwen

#include "stdafx.h"

#include <atlbase.h>

#include "version.h"
#include "FilesHash.h"
#include "AboutDlg.h"
#include "Functions.h"
#include "UIStrings.h"

// CAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
	, m_about(_T(""))
{
}

CAboutDlg::~CAboutDlg()
{
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(ABOUTDLG_TITLE); // 设置标题

	CWnd* pWnd;
	pWnd = GetDlgItem(IDOK);
	pWnd->SetWindowText(BUTTON_OK);
	pWnd = GetDlgItem(IDC_SYSLINK_SITE);
	pWnd->SetWindowText(ABOUTDLG_PROJECT_SITE);

	
	CString fHashVersion = STR_VERSION; //GetExeFileVersion(exeFullPath);

	CString osinfo = GetWindowsInfo();
	m_about = ABOUTDLG_INFO_TITLE;
	m_about.Append(fHashVersion);
	m_about.Append("\r\n");
	m_about.Append(ABOUTDLG_INFO_RIGHT);
	m_about.Append("\r\n\r\n");
	m_about.Append(ABOUTDLG_INFO_MD5);
	m_about.Append("\r\n");
	m_about.Append(ABOUTDLG_INFO_SHA256);
	m_about.Append("\r\n");
	m_about.Append(ABOUTDLG_INFO_CRC32);
	/*m_about.Append("\r\n");
	m_about.Append(ABOUTDLG_INFO_RIGHTDETAIL);*/
	m_about.Append("\r\n\r\n");

	//m_about = m_about + "当前操作系统:\r\n" + osinfo;
	m_about.Append(ABOUTDLG_INFO_OSTITLE);
	m_about.Append("\r\n");
	m_about.Append(osinfo);

	UpdateData(FALSE);

	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAIN, m_about);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_SITE, &CAboutDlg::OnNMClickSyslinkSite)
END_MESSAGE_MAP()


// CAboutDlg 消息处理程序

void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	EndDialog(0);
}

void CAboutDlg::OnNMClickSyslinkSite(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, "open", "http://code.google.com/p/fhash/", NULL, NULL, SW_SHOW);

	*pResult = 0;
}
