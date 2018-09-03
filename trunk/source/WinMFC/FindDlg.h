#pragma once

// CFindDialog 对话框

class CFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindDlg)

public:
	CFindDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindDlg();

// 对话框数据
	enum { IDD = IDD_FIND_DIALOG };

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CString GetFindHash();
	void SetFindHash(const CString& str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CString m_strHash;
	
};
