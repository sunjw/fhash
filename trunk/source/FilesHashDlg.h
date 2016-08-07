// MD5SUM03Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "HyperEditHash.h"

#include "strhelper.h"
#include "OsUtils/OsThread.h"

#include "Global.h"
#include "UIBridgeMFC.h"

// CMD5SUM03Dlg 对话框
class CFilesHashDlg : public CDialog
{
// 构造
public:
	CFilesHashDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedClean();
	afx_msg void OnBnClickedCopy();
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedContext();
	afx_msg void OnBnClickedCheckup();
	afx_msg void OnBnClickedUpperHash();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnThreadMsg(WPARAM, LPARAM);
	afx_msg LRESULT OnCustomMsg(WPARAM, LPARAM);
	afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnHypereditmenuCopyhash();
	afx_msg void OnHypereditmenuSearchgoogle();
	afx_msg void OnHypereditmenuSearchvirustotal();
	afx_msg void OnUpdateHypereditmenuCopyhash(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHypereditmenuSearchgoogle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHypereditmenuSearchvirustotal(CCmdUI *pCmdUI);

// 实现
protected:
	HICON m_hIcon;

	CProgressCtrl m_progWhole;
	CHyperEditHash m_editMain;
	CButton m_btnOpen;
	CButton m_btnExit;
	CButton m_btnClr;
	CButton m_btnCopy;
	CButton m_btnFind;
	CButton m_chkUppercase;
	CButton m_btnContext;

	sunjwbase::OsMutex m_mainMtx;

	UIBridgeMFC *m_uiBridgeMFC;
	ThreadData m_thrdData;
	HANDLE m_hWorkThread;
	float m_calculateTime;
	UINT_PTR m_timer;
	BOOL m_waitingExit; // 等待线程退出后，退出程序
	BOOL m_bAdvTaskbar;
	ITaskbarList3* pTl;

	BOOL m_bFind; // 是否在搜索模式
	CString m_strFindFile;
	CString m_strFindHash;

	BOOL m_bLimited;

	TStrVector ParseCmdLine();
	void ClearFilePaths();
	void PrepareAdvTaskbar();

	void ResultFind(CString strFile, CString strHash);

	void AppendResult(const ResultData& result);

	void DoMD5();
	void StopWorkingThread();
	
	// 下面的为计算时使用，一般不要用
	void SetCtrls(BOOL working);
	void SetWholeProgPos(UINT pos);
	void RefreshResult();
	void RefreshMainText(BOOL bScrollToEnd = TRUE);
	void CalcSpeed(ULONGLONG tsize);

};
