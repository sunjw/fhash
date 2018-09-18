//********************************************************************************************
//*	HYPEREDIT.CPP - Custom window message handler(s) implementation                          *
//*                                                                                          *
//* This module contains implementation code for object initialization and default windows   *
//* messages.                                                                                *
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
//* 7.8.2016 - Sun Junwen                                                                    *
//* Change for fHash usage.                                                                  *
//********************************************************************************************

#include "stdafx.h"
#include "HyperEdit.h"
#include "winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CHyperEdit, CEdit)

// Object construction

CHyperEdit::CHyperEdit()
{
	m_nLineHeight = 0;

	m_bMouseOnHyperlink = FALSE;

	// Set default colors
	m_crEditBk = RGB(255, 255, 255);
	m_crHyperlinkNormal = RGB(92, 92, 154);
	m_crHyperlinkHover = RGB(168, 168, 230);

	m_csLocation.Empty();
}

// Object destruction

CHyperEdit::~CHyperEdit()
{
	m_oFont.DeleteObject();	// Delete hyperlink font object
	m_brEditBk.DeleteObject();
}
		  
BEGIN_MESSAGE_MAP(CHyperEdit, CEdit)
	//{{AFX_MSG_MAP(CHyperEdit)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Control initialization

void CHyperEdit::PreSubclassWindow() 
{
	CEdit::PreSubclassWindow();
	
	ASSERT(GetStyle() & ES_MULTILINE); // MUST be in multiline state
	ASSERT(GetStyle() & WS_VSCROLL); // We need vertical scrollbar
	//ASSERT(GetStyle() & ES_AUTOVSCROLL);

	// Initialize font object(s)
	CDC* pDC = GetDC();
	ASSERT(pDC);

	CFont* pTemp = GetFont(); // Get the font dialog controls will be using
	ASSERT(pTemp);

	LOGFONT lf;
	pTemp->GetLogFont(&lf);

	// TODO:
	// Check if there is a system setting that we can check to see if hyperlinks should be 
	// underlined by default or not.
	lf.lfUnderline = TRUE; // Our font needs to be undelrined to distinguish itself as hyper

	// Create our hyperlink font :)
	m_oFont.CreateFontIndirect(&lf);

	// Calculate single line height
	m_nLineHeight = pDC->DrawText(_T("Test Line"), CRect(0,0,0,0), DT_SINGLELINE|DT_CALCRECT);

}

int CHyperEdit::GetFirstInvisibleLine()
{
	CRect rcRect;
	GetRect(rcRect);
	int iLastVisibleLine = GetFirstVisibleLine();
	int iLineCount = GetLineCount();
	for (; iLastVisibleLine < iLineCount; ++iLastVisibleLine)
	{
		int iChar = LineIndex(iLastVisibleLine);
		CPoint ptChar = PosFromChar(iChar);
		if (ptChar.y > rcRect.bottom)
			break;
	}

	if (iLastVisibleLine >= iLineCount)
		iLastVisibleLine = iLineCount - 1;

	return iLastVisibleLine;
}

// Override mouse movements

void CHyperEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_bMouseOnHyperlink = FALSE;

	CString csURL = GetHyperlinkFromPoint(point);
	if (!csURL.IsEmpty())
		m_bMouseOnHyperlink = TRUE;

	CEdit::OnMouseMove(nFlags, point);
}

BOOL CHyperEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCAPTION)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}

	CPoint ptMouse(GetMessagePos()); // Current mouse location
	ScreenToClient(&ptMouse);

	CRect rectEdit;
	GetRect(rectEdit); // Get the formatting rectangle for the edit control
	if (ptMouse.x < rectEdit.left ||
		ptMouse.x > rectEdit.right ||
		ptMouse.y < rectEdit.top ||
		ptMouse.y > rectEdit.bottom)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}

	if (m_bMouseOnHyperlink)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));

	return TRUE;
}

HBRUSH CHyperEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	m_brEditBk.DeleteObject();
	m_brEditBk.CreateSolidBrush(m_crEditBk);
	return (HBRUSH)m_brEditBk.GetSafeHandle();
}

// Override left mouse button down (Clicking)

void CHyperEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_csLocation = GetHyperlinkFromPoint(point);

	CEdit::OnLButtonDown(nFlags, point);
}

// Override left mouse button up (Clicking)

void CHyperEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonUp(nFlags, point);

	int iSelStart=0, iSelFinish=0; 
	GetSel(iSelStart, iSelFinish);

	// If there is a selection, just exit now, we don't open URL's
	if(IsSelection(iSelStart, iSelFinish)) return;

	CString csURL = GetHyperlinkFromPoint(point);

	// If not empty, then open browser and show web site
	// only if the URL is the same as one clicked on in OnLButtonDown()
	if(!csURL.IsEmpty() && (m_csLocation==csURL))
	{
		OpenHyperlink(csURL, point);
		m_bMouseOnHyperlink = FALSE;
	}
}

// Override low level message handling

LRESULT CHyperEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// handling OnPaint() didn't work
	if (message == WM_PAINT)
	{
		CEdit::WindowProc(message, wParam, lParam);

		DrawHyperlinks();

		return TRUE;
	}
	
	return CEdit::WindowProc(message, wParam, lParam);
}

//
// DrawHyperlinks() draws a hyperlink colored character(individually) over existing
// CEdit text entered by user. It does this by first building a internal offset list of all
// valid hyperlinks and then iterating over each of them and their individual characters
// painting them the color specified by the client programmer or using default colors.
//

void CHyperEdit::DrawHyperlinks()
{
	CRect rcRect;
	GetRect(rcRect); // Get the formatting rectangle for the edit control

	CDC* pDC = GetDC(); 
	ASSERT(pDC);

	// Select our hyperlink font into the DC
	CFont* pTemp = pDC->SelectObject(&m_oFont);

	// Prepare the DC for for our output on top of existing content
	pDC->SetBkMode(TRANSPARENT);
	pDC->IntersectClipRect(rcRect);	// Prevent drawing outside the format rectangle

	// Get the character index of the first and last visible characters
	//int iCharStart = LOWORD(CharFromPos(CPoint(rcRect.left, rcRect.top))); // LineIndex(GetFirstVisibleLine()); // Old method!!!
	//int iCharFinish = LOWORD(CharFromPos(CPoint(rcRect.right, rcRect.bottom)));

	int iCharStart = 0;
	int iCharFinish = 0;

	int iVisibleLine = GetFirstVisibleLine();
	iCharStart = LineIndex(iVisibleLine);

	int iInvisibleLine = GetFirstInvisibleLine();
	iCharFinish = LineIndex(iInvisibleLine);

	// Build a list of hyperlink character offsets
	BuildOffsetList(iCharStart, iCharFinish);

	CString csBuff; // Holds the text for the entire control
	GetWindowText(csBuff);

	// Holds the start and finish offset of current selection (if any)
	int iSelStart=0, iSelFinish=0;
	GetSel(iSelStart, iSelFinish);

	// Used to determine if user is hovering over a hyperlink or not
	//CPoint ptMouse(GetMessagePos()); // Current mouse location
	//ScreenToClient(&ptMouse);

	//CString csTemp; //

	// Draw our hyperlink(s)	
	for (int i = 0; i < m_linkOffsets.size(); i++)
	{
		if (m_linkOffsets[i].iStart + m_linkOffsets[i].iLength < iCharStart ||
			m_linkOffsets[i].iStart > iCharFinish)
			continue; // Do not draw outside

		// Determine if mouse pointer is over a hyperlink
		//csTemp = GetHyperlinkFromPoint(ptMouse);

		pDC->SetTextColor(m_crHyperlinkNormal);

		// If return URL is empty then were not over a hyperlink
		//if (!csTemp.IsEmpty() &&
		//	csTemp == csBuff.Mid(m_linkOffsets[i].iStart, m_linkOffsets[i].iLength))
		//	pDC->SetTextColor(m_crHyperlinkHover);

		// Paint each URL, email, etc character individually so we can have URL's that wrap
		// onto different lines
		for (int j = m_linkOffsets[i].iStart; 
			j < (m_linkOffsets[i].iStart + m_linkOffsets[i].iLength); 
			j++)
		{
			CPoint pt; // Coordinates for a single tokens character which is painted blue
			TCHAR chToken = csBuff.GetAt(j); // Get a single token from URL, Email, etc
			pt = PosFromCharEx(j); // Get the coordinates for a single token

			BOOL needDraw = FALSE;
			// Determine if there is a selection
			if (IsSelection(iSelStart, iSelFinish))
			{
				// Does our current token fall within a selection range
				if (j < iSelStart || j >= iSelFinish)
					needDraw = TRUE;
			}
			else // No selection, just draw normally
			{
				needDraw = TRUE;
			}

			if (needDraw)
			{
				CString cstrToken = CString(chToken);

				CSize size = pDC->GetTextExtent(cstrToken);
				if (j == m_linkOffsets[i].iStart)
				{
					// 1st fix
					pDC->FillSolidRect(pt.x - 1, pt.y, size.cx + 1, size.cy, m_crEditBk);
				}
				else if (j == m_linkOffsets[i].iStart + m_linkOffsets[i].iLength - 1)
				{
					// Last fix
					pDC->FillSolidRect(pt.x, pt.y, size.cx + 1, size.cy, m_crEditBk);
				}
				else
				{
					pDC->FillSolidRect(pt.x, pt.y, size.cx, size.cy, m_crEditBk);
				}
				// Draw overtop of existing character
				pDC->TextOut(pt.x, pt.y, cstrToken);
			}
		}
	}

	pDC->SelectObject(pTemp); // Restore original font (Free hyperlink font)
}

//
// Builds an offset list of all valid hyperlinks. This function is optimized by only
// searching through characters which are visible, ignoring those that would require 
// scrolling to view. When this function encounters an token and it determines it's
// hyperlink-able (Using virtual IsWordHyperlink) the starting offset and length of where
// the hyperlink exists within the actual CEdit buffer is recorded and pushed onto a vector
//

void CHyperEdit::BuildOffsetList(int iCharStart, int iCharFinish)
{
	// Entire control buffer and individual token buffer
	CString csBuff, csToken; 
	GetWindowText(csBuff);

	// Clear previous hyperlink offsets from vector and rebuild list
	m_linkOffsets.clear(); 

	// Rebuild list of hyperlink character offsets starting at iCharStart and ending at iCharFinish
	for(int i=iCharStart, iCurr=iCharStart; i<=iCharFinish; i++){
		
		if(IsWhiteSpaceOrEOF(csBuff, i)){	// Also checks for EOB (End of buffer)

			_TOKEN_OFFSET off = { iCurr /* Start offset */, i-iCurr /* Length */ };

			// Let client programmer define what tokens can be hyperlinked or not
			// if one desires he/she could easily implement an easy check using a
			// regex library on email addresses without using the mailto: suffix
			if(IsWordHyperlink(csToken))
				m_linkOffsets.push_back(off); // Save the starting offset for current token

			csToken.Empty(); // Flush previous token 		
			iCurr = i+1; // Initialize the start offset for next token
		}
		else 
			csToken += csBuff.GetAt(i); // Concatenate another char onto token
	}
}

CPoint CHyperEdit::PosFromCharEx(UINT nChar)
{
	if (nChar < (UINT)GetWindowTextLength())
	{
		return PosFromChar(nChar);
	}
	else
	{
		// PosFromChar has a bug on over last char.
		// https://social.msdn.microsoft.com/Forums/en-US/5740af95-ec61-4f6a-b46b-ad22521e4609/cedit-posfromchar-broken
		CPoint point = PosFromChar(GetWindowTextLength() - 1); // index of last character
		CDC *pDC = GetDC();
		CString str;
		GetWindowText(str);
		str = str.Right(1); // last character in string
		point.x += pDC->GetTextExtent(str).cx; // bump x by it's width
		return point;
	}
}

//
// Returns a hyperlinks URL if mouse cursor is actually over a URL
// and if mouse isn't over any hyperlink it returns a empty CString
//

CString CHyperEdit::GetHyperlinkFromPoint(CPoint& pt)
{
	CString csBuff, csTemp;
	GetWindowText(csBuff);

	int iCharStart = 0;
	int iCharFinish = 0;

	int iVisibleLine = GetFirstVisibleLine();
	iCharStart = LineIndex(iVisibleLine);

	int iInvisibleLine = GetFirstInvisibleLine();
	iCharFinish = LineIndex(iInvisibleLine);

	size_t linkOffsetsCount = m_linkOffsets.size();
	for (size_t i = 0; i < linkOffsetsCount; ++i)
	{
		size_t lastIdx = linkOffsetsCount - 1 - i;
		const _TOKEN_OFFSET& linkOffset = m_linkOffsets[lastIdx];

		if (linkOffset.iStart + linkOffset.iLength < iCharStart ||
			linkOffset.iStart > iCharFinish)
			continue;

		CPoint linkLTPoint = PosFromCharEx(linkOffset.iStart);
		CPoint linkRTPoint = PosFromCharEx(linkOffset.iStart + linkOffset.iLength);

		if (pt.y > linkLTPoint.y &&
			pt.y < linkLTPoint.y + m_nLineHeight &&
			pt.x >= linkLTPoint.x &&
			pt.x <= linkRTPoint.x)
		{
			csTemp = csBuff.Mid(linkOffset.iStart, linkOffset.iLength);
			return csTemp;
		}
	}

	csTemp.Empty(); // NULL string on error
	return csTemp; 
}

//
// Returns TRUE if indexed character within buffer is whitespace
//

BOOL CHyperEdit::IsWhiteSpaceOrEOF(const CString& csBuff, int iIndex) const 
{ 
	// Check for End of buffer 
	if(csBuff.GetLength() == iIndex) return TRUE;
		
	// Check for whitespace
	if(csBuff.GetAt(iIndex) == WHITE_SPACE1) return TRUE;
	if(csBuff.GetAt(iIndex) == WHITE_SPACE2) return TRUE;
	if(csBuff.GetAt(iIndex) == WHITE_SPACE3) return TRUE;
	if(csBuff.GetAt(iIndex) == WHITE_SPACE4) return TRUE;

	return FALSE;
}

//
// Virtual function which can be overridden by client programmer to allow advanced
// hyperlinking. For example in a derived class we could use regex to validate
// email addresses without using the 'mailto:' suffix.
//

BOOL CHyperEdit::IsWordHyperlink(const CString& csToken) const
{
	CString csTemp(csToken); // Make a temp copy so we can convert it's case
	csTemp.MakeLower();

	// A trivial approach to hyperlinking web sites or email addresses
	// In a derived class we can use regex if we like to only hyperlink
	// fully qualified URL's etc...
	if(csTemp.Left(7) == _T("http://")) return TRUE;
	if(csTemp.Left(7) == _T("mailto:")) return TRUE;
	//if(csTemp.Left(5) == "file:") return TRUE;

	return FALSE; // Not a valid token by default
}	  

HINSTANCE CHyperEdit::OpenHyperlink(const CString& hyperlink, CPoint point)
{
	return GotoURL(hyperlink);
}

HINSTANCE CHyperEdit::GotoURL(const CString& url) const
{
	HINSTANCE result = ShellExecute(NULL, _T("open"), url.GetString(), NULL, NULL, SW_SHOW);

	return result;
}
