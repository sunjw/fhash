// FindDialog.cpp : 实现文件
//

#include "stdafx.h"

#include "FilesHash.h"
#include "FindDlg.h"
#include "WinCommon/WindowsStrings.h"

// CFindDlg 对话框

IMPLEMENT_DYNAMIC(CFindDlg, CDialog)

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
	, m_strHash(_T(""))
{

}

CFindDlg::~CFindDlg()
{
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HASH, m_strHash);
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFindDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(GetStringByKey(FINDDLG_TITLE));

	CWnd* pWnd;
	pWnd = GetDlgItem(IDOK);
	pWnd->SetWindowText(GetStringByKey(BUTTON_OK));
	pWnd = GetDlgItem(IDCANCEL);
	pWnd->SetWindowText(GetStringByKey(BUTTON_CANCEL));
	pWnd = GetDlgItem(IDC_STATIC_HASH);
	pWnd->SetWindowText(GetStringByKey(HASHVALUE_STRING));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CFindDialog 消息处理程序
void CFindDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	OnOK();
}

CString CFindDlg::GetFindHash()
{
	return m_strHash;
}

void CFindDlg::SetFindHash(const CString& str)
{
	m_strHash = str;
}
