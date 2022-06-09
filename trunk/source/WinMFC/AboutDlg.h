#pragma once
#include "afxwin.h"

// CAboutDlg 对话框

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CString m_about;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickSyslinkSite(NMHDR *pNMHDR, LRESULT *pResult);

//private:
//	bool AddContextMenu(void);
//	bool RemoveContextMenu(void);
//	bool IsContextMenuAdded(void);
//
//	char exeFullPath[MAX_PATH + 10]; // MAX_PATH
};
