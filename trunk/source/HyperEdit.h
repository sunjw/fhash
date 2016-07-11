//********************************************************************************************
//*	HYPEREDIT.H - Control interface and trivial implementation                               *
//*                                                                                          *
//* This module contains interface code and some typedefs and application defines            *
//*                                                                                          *
//* Aug.31.04                                                                                *
//*                                                                                          *
//* Copyright PCSpectra 2004 (Free for any purpose, except to sell indivually)               *
//* Website: www.pcspectra.com                                                               *
//*                                                                                          *
//* Notes:                                                                                   *
//* ======																					 *
//* Search module for 'PROGRAMMERS NOTE'                                                     *
//*                                                                                          *
//* History:						                                                         *
//*	========																				 *
//* Mon.dd.yy - None so far     														     *
//********************************************************************************************

#ifndef AFX_HYPEREDIT_H__28F52ED8_8811_436F_821B_EB02D02A1F88__INCLUDED_
#define AFX_HYPEREDIT_H__28F52ED8_8811_436F_821B_EB02D02A1F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WHITE_SPACE1   _T(' ')   // Single space
#define WHITE_SPACE2   _T('\r')  // Carriage return
#define WHITE_SPACE3   _T('\n')  // Linefeed/newline
#define WHITE_SPACE4   _T('\t')  // Tab

#define IDT_SELCHANGE 0x6969 

#define IsSelection(p1, p2)( p1 != p2 )

#include <vector>

//
// This structure holds the starting offset and length for each token
// located inside the buffer for the control. We use this data inside
// OnPaint and OnMouseMove and OnChange to quickly get the URL or token string
// 

struct _TOKEN_OFFSET
{
	INT iStart;
	INT iLength;
};

typedef std::vector<_TOKEN_OFFSET> OFFSETS;

// CHyperEdit control interface

class CHyperEdit : public CEdit
{
	DECLARE_DYNAMIC(CHyperEdit)

public:
	CHyperEdit();
	virtual ~CHyperEdit();

	int GetFirstInvisibleLine();

	COLORREF GetBackgroundColor() const { return m_crEditBk; }
	COLORREF GetHyperlinkNormalColor() const { return m_crHyperlinkNormal; }
	COLORREF GetHyperlinkHoverColor() const { return m_crHyperlinkHover; }

	void SetBackgroundColor(COLORREF crBackground)
	{ m_crEditBk = crBackground; }

	void SetHyperlinkColors(COLORREF crNormal, COLORREF crHover)
	{ m_crHyperlinkNormal = crNormal; m_crHyperlinkHover = crHover; }

	CPoint PosFromCharEx(UINT nChar);

	CString GetHyperlinkFromPoint(CPoint& pt);

protected:
	//{{AFX_VIRTUAL(CHyperEdit)
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHyperEdit)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	///

	inline BOOL IsWhiteSpaceOrEOF(const CString& csBuff, int iIndex) const;

	virtual void BuildOffsetList(int iCharStart, int iCharFinish);

	virtual HINSTANCE OpenHyperlink(const CString& hyperlink, CPoint point);

	OFFSETS m_linkOffsets; // Character offsets for each hyperlink located

	///

private:
	DECLARE_MESSAGE_MAP()

	///

	BOOL IsWordHyperlink(const CString& csToken) const;

	void DrawHyperlinks();

	// Functions borrowed from Chris Maunder's article
	HINSTANCE GotoURL(const CString& url) const;

	///

private:
	INT m_nLineHeight;
	
	CFont m_oFont;

	CString m_csLocation; // URL or file to be opened when mouse is released

	BOOL m_bMouseOnHyperlink;

	COLORREF m_crEditBk;
	CBrush m_brEditBk;

	COLORREF m_crHyperlinkNormal;
	COLORREF m_crHyperlinkHover;

};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_HYPEREDIT_H__28F52ED8_8811_436F_821B_EB02D02A1F88__INCLUDED_)
