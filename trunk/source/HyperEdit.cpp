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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CHyperEdit, CEdit)

// Object construction

CHyperEdit::CHyperEdit()
{
	m_nTimer = 0;

	m_nLineHeight = 0;

	m_hHandCursor = NULL; // No hand cursor 

	// Set default hyperlink colors
	m_clrNormal = RGB(92, 92, 154);
	m_clrHover = RGB(168, 168, 230);

	m_csLocation.Empty();
}

// Object destruction

CHyperEdit::~CHyperEdit()
{
	m_oFont.DeleteObject();	// Delete hyperlink font object
}
		  
BEGIN_MESSAGE_MAP(CHyperEdit, CEdit)
	//{{AFX_MSG_MAP(CHyperEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
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

	InitHandCursor(); // Try and initialize our hand cursor

	// Calculate single line height
	m_nLineHeight = pDC->DrawText(_T("Test Line"), CRect(0,0,0,0), DT_SINGLELINE|DT_CALCRECT);

	// PROGRAMMERS NOTE:
	// =================
	// If the hyperlinks flicker when changing the selection state
	// of the edit control change the timer value to a lower count
	// 10 is the default and appears to have an almost flicker free
	// transition from selection to hyperlinked colors.
	m_nTimer = SetTimer(IDT_SELCHANGE, 10, NULL);
}

// Override mouse movements

void CHyperEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	CEdit::OnMouseMove(nFlags, point);
									  
	CString csURL = GetHyperlinkFromPoint(point);

	// If not empty, then display hand cursor
	if(!csURL.IsEmpty()){
	
		// Get the coordinates of last character in entire buffer
		CPoint pt_lastchar = PosFromChar(GetWindowTextLength()-1);

		// Don't bother changing mouse cursor if it's below last visible character
		if(point.y<=(pt_lastchar.y+m_nLineHeight))		
			::SetCursor(m_hHandCursor);
	}

	DrawHyperlinks();
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

	// If were below the last visible character	exit again, cuz we don't want 
	// to open any URL's that aren't directly clicked on
	// Get the coordinates of last character in entire buffer
	CPoint pt = PosFromChar(GetWindowTextLength()-1);

	// Exit if mouse is below last visible character
	if(point.y>(pt.y+m_nLineHeight)) return; 

	CString csURL = GetHyperlinkFromPoint(point);

	// If not empty, then open browser and show web site
	// only if the URL is the same as one clicked on in OnLButtonDown()
	if(!csURL.IsEmpty() && (m_csLocation==csURL)) 
		OpenHyperlink(csURL, SW_SHOW);
}

// Override low level message handling

LRESULT CHyperEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// handling OnPaint() didn't work
	if(message==WM_PAINT){
		CEdit::WindowProc(message, wParam, lParam);

		DrawHyperlinks();

		return TRUE;
	}
	
	return CEdit::WindowProc(message, wParam, lParam);
}

// Emulate an OnSelChange() event using a low interval timer (initialized in PreSubclassWindow)

void CHyperEdit::OnTimer(UINT_PTR nIDEvent) 
{
	//
	// Emulate a OnSelChange() event
	//

	static int iPrevStart=0, iPrevFinish=0;
	
	DWORD dwSel = GetSel();	

	// Check the previous start/finish of selection range
	// and compare them against the current selection range
	// if there is any difference between them fire off an OnSelChange event
	if(LOWORD(dwSel) != iPrevStart || HIWORD(dwSel) != iPrevFinish)
		OnSelChange();

	// Save current selection state for next call (as previous state)
	iPrevStart = LOWORD(dwSel);
	iPrevFinish = HIWORD(dwSel);

	CEdit::OnTimer(nIDEvent);
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
	int iChar1 = LOWORD(CharFromPos(CPoint(rcRect.left, rcRect.top))); // LineIndex(GetFirstVisibleLine()); // Old method!!!
	int iChar2 = LOWORD(CharFromPos(CPoint(rcRect.right, rcRect.bottom)));

	// Build a list of hyperlink character offsets
	BuildOffsetList(iChar1, iChar2);

	CString csBuff; // Holds the text for the entire control
	GetWindowText(csBuff);

	CPoint pt; // Coordinates for a single tokens character which is painted blue

	// Used to determine if user is hovering over a hyperlink or not
	CPoint pt_mouse(GetMessagePos()); // Current mouse location
	ScreenToClient(&pt_mouse);

	CString csTemp; //

	// Draw our hyperlink(s)	
	for(int i=0; i<m_linkOffsets.size(); i++){
		   
		// Determine if mouse pointer is over a hyperlink
		csTemp = GetHyperlinkFromPoint(pt_mouse);
			
		// If return URL is empty then were not over a hyperlink
		if(csTemp.IsEmpty())		
			pDC->SetTextColor(m_clrNormal);
		else{
			// Make sure we only hilite the URL were over. This technique will
			// cause duplicate URl's to hilite in hover color.
			if(csTemp==csBuff.Mid(m_linkOffsets[i].iStart, m_linkOffsets[i].iLength)){
				
				// Get the coordinates of last character in entire buffer
				CPoint pt_lastchar = PosFromChar(GetWindowTextLength()-1);

				// Paint normally if mouse is below last visible character
				if(pt_mouse.y>(pt_lastchar.y+m_nLineHeight))
					pDC->SetTextColor(m_clrNormal);
				else
					pDC->SetTextColor(m_clrHover);
			}
			else
				pDC->SetTextColor(m_clrNormal);
		}

		// Paint each URL, email, etc character individually so we can have URL's that wrap
		// onto different lines
		for(int j=m_linkOffsets[i].iStart; j<(m_linkOffsets[i].iStart+m_linkOffsets[i].iLength); j++){
			
			TCHAR chToken = csBuff.GetAt(j); // Get a single token from URL, Email, etc
			pt = PosFromChar(j); // Get the coordinates for a single token

			// Holds the start and finish offset of current selection (if any)
			int iSelStart=0, iSelFinish=0; 

			GetSel(iSelStart, iSelFinish);

			// Determine if there is a selection
			if(IsSelection(iSelStart, iSelFinish)){
				// Does our current token fall within a selection range
				if(j>=iSelStart && j<iSelFinish)
					continue; // Don't paint token blue, it's selected!!!
				else
					pDC->TextOut(pt.x, pt.y, CString(chToken)); // Draw overtop of existing character
			}
			else // No selection, just draw normally
				pDC->TextOut(pt.x, pt.y, CString(chToken)); // Draw overtop of existing character
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

	// Rebuild list of hyperlink character offsets starting at iChar1 and ending at iChar2
	for(int i=iCharStart, iCurr=iCharStart; i<=iCharFinish; i++){
		
		if(IsWhiteSpace(csBuff, i)){	// Also checks for EOB (End of buffer)

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

//
// Returns a hyperlinks URL if mouse cursor is actually over a URL
// and if mouse isn't over any hyperlink it returns a empty CString
//

CString CHyperEdit::GetHyperlinkFromPoint(CPoint& pt) const 
{
	CString csBuff, csTemp;
	GetWindowText(csBuff);

	// Get the index of the character caret is currently over or closest too
	int iChar = LOWORD(CharFromPos(pt));

	// Check 'iChar' against vector offsets and determine if current character
	// user is hovering over is inside any hyperlink range
	for(int i=0; i<m_linkOffsets.size(); i++){
			
		// If character user is over is within range of this token URL, let's exit and send the URL
		if(iChar>=m_linkOffsets[i].iStart && iChar<(m_linkOffsets[i].iStart+m_linkOffsets[i].iLength)){
			csTemp = csBuff.Mid(m_linkOffsets[i].iStart, m_linkOffsets[i].iLength);
			return csTemp;
		}
	}

	csTemp.Empty(); // NULL string on error
	return csTemp; 
}

//
// Returns TRUE if indexed character within buffer is whitespace
//

BOOL CHyperEdit::IsWhiteSpace(const CString& csBuff, int iIndex) const 
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
	if(IsWhiteSpace(csToken, 0)) return FALSE; // Whitespace YUCK!!!

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

HINSTANCE CHyperEdit::OpenHyperlink(LPCTSTR hyperlink, int showcmd)
{
	return GotoURL(hyperlink, showcmd);
}

void CHyperEdit::InitHandCursor()
{
    if(m_hHandCursor == NULL){ // No cursor handle - load our own
        // Get the windows directory
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");

        // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
		HMODULE hModule = LoadLibrary(strWndDir);

        if(hModule){
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            
			if(hHandCursor)
                m_hHandCursor = CopyCursor(hHandCursor);
        }

        FreeLibrary(hModule);
    }
}

LONG CHyperEdit::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}

HINSTANCE CHyperEdit::GotoURL(LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) {

        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
            lstrcat(key, _T("\\shell\\open\\command"));

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = _tcsstr(key, _T("%1"));      // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);

                USES_CONVERSION;
                result = (HINSTANCE) WinExec(T2A(key),showcmd);
            }
        }
    }

    return result;
}
