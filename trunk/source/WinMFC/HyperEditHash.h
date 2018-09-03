#pragma once

#include "HyperEdit.h"

// CHyperEditHash
#define CHyperEditHash_BASE_CLASS CHyperEdit

class CHyperEditHash : public CHyperEditHash_BASE_CLASS
{
	DECLARE_DYNAMIC(CHyperEditHash)

public:
	CHyperEditHash();
	virtual ~CHyperEditHash();

	void ClearTextBuffer();
	CString GetTextBuffer() const;

	void ShowTextBuffer();

	void AppendTextToBuffer(LPCTSTR pszText);
	void AppendLinkToBuffer(LPCTSTR pszText);

	void CopyLinkOffsets(OFFSETS& linkOffsets);
	void SetLinkOffsets(const OFFSETS& linkOffsets);

	inline CString GetLastHyperlink() const
	{ return m_cstrLastHyperlink; }

	inline CPoint GetLastScreenPoint() const
	{ return m_cpLastScreenPoint; }

protected:
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);

	virtual void BuildOffsetList(int iCharStart, int iCharFinish);

	virtual HINSTANCE OpenHyperlink(const CString& hyperlink, CPoint point);

private:
	CString m_cstrTextBuffer;

	CRITICAL_SECTION m_csLinkOffsets;
	OFFSETS m_bufferLinkOffsets;

	CString m_cstrLastHyperlink;
	CPoint m_cpLastScreenPoint;

};


