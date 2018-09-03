// AboutDlg.cpp : 实现文件
// SUN Junwen

#include "stdafx.h"

#include <atlbase.h>

#include "version.h"
#include "FilesHash.h"
#include "AboutDlg.h"
#include "WindowsUtils.h"
#include "WindowsStrings.h"

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

	SetWindowText(GetStringByKey(ABOUTDLG_TITLE)); // 设置标题

	CWnd* pWnd;
	pWnd = GetDlgItem(IDOK);
	pWnd->SetWindowText(GetStringByKey(BUTTON_OK));
	pWnd = GetDlgItem(IDC_SYSLINK_SITE);
	pWnd->SetWindowText(GetStringByKey(ABOUTDLG_PROJECT_SITE));

	
	CString fHashVersion = _T(STR_VERSION); //GetExeFileVersion(exeFullPath);

	CString osinfo = WindowsUtils::GetWindowsInfo();
	m_about = GetStringByKey(ABOUTDLG_INFO_TITLE);
	m_about.Append(fHashVersion);
	if(sizeof(void*) == 8)
		m_about.Append(_T(" x64"));
	m_about.Append(_T("\r\n"));
	m_about.Append(GetStringByKey(ABOUTDLG_INFO_RIGHT));
	m_about.Append(_T("\r\n\r\n"));
	m_about.Append(GetStringByKey(ABOUTDLG_INFO_MD5));
	m_about.Append(_T("\r\n"));
	m_about.Append(GetStringByKey(ABOUTDLG_INFO_SHA256));
	m_about.Append(_T("\r\n"));
	m_about.Append(GetStringByKey(ABOUTDLG_INFO_CRC32));
	/*m_about.Append(_T("\r\n"));
	m_about.Append(GetStringByKey(ABOUTDLG_INFO_RIGHTDETAIL));*/
	m_about.Append(_T("\r\n\r\n"));

	//m_about = m_about + "当前操作系统:\r\n" + osinfo;
	m_about.Append(GetStringByKey(ABOUTDLG_INFO_OSTITLE));
	m_about.Append(_T("\r\n"));
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
	WindowsUtils::OpenURL(GetStringByKey(ABOUTDLG_PROJECT_URL));

	*pResult = 0;
}
